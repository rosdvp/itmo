#include <exception>
#include <iostream>
#include <Windows.h>

#include "CoalesceAllocator.h"
#include "AllocatorsHelpers.h"

CoalesceAllocator::CoalesceAllocator(size_t osPageSize, size_t allocatorPageSize) :
	_pageSize(AlignSizeIncreasing(allocatorPageSize, osPageSize)),
	_page(nullptr),
	_cursor(nullptr)
{}

CoalesceAllocator::CoalesceAllocator(size_t allocatorPageSize) :
	CoalesceAllocator(GetOSVirtualPageSize(), allocatorPageSize)
{}

CoalesceAllocator::~CoalesceAllocator()
{
	_ASSERT_EXPR(_page == nullptr, "Allocator is not destroyed");
}

void CoalesceAllocator::Init()
{
	_ASSERT_EXPR(_page == nullptr, "Allocator is already initialized");

	_page = InsertNewPage(nullptr);
	_cursor = WriteBlockHeaderAndFooter(_page->Start, false, _pageSize - sizeof(Page));
	PutInFreeList(_cursor, _cursor, _cursor);
}

void CoalesceAllocator::Destroy()
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	_ASSERT_EXPR(!_debugAllocStats.IsAnyAllocatedNow(), "Not all blocks are free");

	Page* selected = _page;
	do
	{
		Page* next = selected->NextPage;
		VirtualFree(selected, 0, MEM_RELEASE);
		selected = next;
	} while (selected != _page);
	_page = nullptr;

#if _DEBUG
	_debugAllocStats.Clear();
#endif
}

void* CoalesceAllocator::Alloc(short contentSize)
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");

	contentSize = AlignSizeIncreasing(contentSize, 8);

	short requiredBlockSize = (contentSize < BLOCK_MIN_SIZE ? BLOCK_MIN_SIZE : contentSize) + sizeof(BlockMeta) * 2;

	_ASSERT_EXPR(requiredBlockSize <= _pageSize - sizeof(Page*), "Required block size is bigger than page size.");
	

	PutCursorOnFirstFitFreeBlock(requiredBlockSize);
	BlockMeta* selectedHeader = _cursor;
	FreeListEntry* selectedFLEntry = GetBlockFreeListEntry(_cursor);

	short remainSize = selectedHeader->Size - requiredBlockSize;
	if (remainSize >= BLOCK_MIN_SIZE)
	{
		//split block since not full size of it is required, and put cursor on new one since it is free
		auto rawCursor = reinterpret_cast<char*>(_cursor);
		rawCursor += requiredBlockSize;
		_cursor = WriteBlockHeaderAndFooter(rawCursor, false, remainSize);
		if (selectedFLEntry->NextFreeHeader == selectedHeader)
		{
			//it was the only free block, so new block will be the only one too
			PutInFreeList(_cursor, _cursor, _cursor);
		}
		else
		{
			//new block should replace old one in free list
			PutInFreeList(_cursor, selectedFLEntry->PrevFreeHeader, selectedFLEntry->NextFreeHeader);
		}
		WriteBlockHeaderAndFooter(reinterpret_cast<char*>(selectedHeader), true, requiredBlockSize);
	}
	else
	{
		WriteBlockHeaderAndFooter(reinterpret_cast<char*>(selectedHeader), true, selectedHeader->Size);
		if (selectedFLEntry->NextFreeHeader == selectedHeader)
		{
			//it was the only free block, so alloc new page and add its block to free list
			_page = InsertNewPage(_page);
			_cursor = WriteBlockHeaderAndFooter(_page->Start, true, _pageSize - sizeof(Page));
			PutInFreeList(_cursor, _cursor, _cursor);
		}
		else
		{
			//there is another free block, so put cursor on it
			RemoveFromFreeList(selectedFLEntry);
			_cursor = selectedFLEntry->NextFreeHeader;
		}
	}

#if _DEBUG
	_debugAllocStats.RecordAlloc(selectedHeader->Size);
#endif

	return GetBlockContent(selectedHeader);
}

bool CoalesceAllocator::TryFree(void* p)
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");

	if (!IsPtrInPages(p))
		return false;

	const auto blockStart = static_cast<char*>(p) - sizeof(BlockMeta);
	auto header = reinterpret_cast<BlockMeta*>(blockStart);

#if _DEBUG
	_debugAllocStats.RecordFree(header->Size);
#endif

	BlockMeta* prevHeader = nullptr;
	BlockMeta* nextHeader = nullptr;

	// try get prev block if it exists and is free
	if (blockStart - sizeof(BlockMeta) > _page->Start)
	{
		auto prevFooter = reinterpret_cast<BlockMeta*>(blockStart - sizeof(BlockMeta));
		if (!prevFooter->IsBusy)
			prevHeader = reinterpret_cast<BlockMeta*>(blockStart - prevFooter->Size);
	}
	// try get next block if it exists and is free
	if (blockStart + header->Size < _page->End)
	{
		nextHeader = reinterpret_cast<BlockMeta*>(blockStart + header->Size);
		if (nextHeader->IsBusy)
			nextHeader = nullptr;
	}

	if (prevHeader == nullptr && nextHeader == nullptr)
	{
		// no blocks to coalesce with, just free current 
		PutInFreeList(header, _cursor, GetBlockFreeListEntry(_cursor)->NextFreeHeader);
		WriteBlockHeaderAndFooter(blockStart, false, header->Size);
	}
	else
	{
		char* finalBlockStart = prevHeader != nullptr ? reinterpret_cast<char*>(prevHeader) : blockStart;
		short finalBlockSize = header->Size +
			(prevHeader == nullptr ? 0 : prevHeader->Size) +
			(nextHeader == nullptr ? 0 : nextHeader->Size);

		FreeListEntry* flEntryToReplace;
		if (prevHeader != nullptr && nextHeader != nullptr)
		{
			// coalesce with both prev block and next block
			// remove next block from free list and take free-list entry of prev block for resulting block
			RemoveFromFreeList(GetBlockFreeListEntry(nextHeader));
			flEntryToReplace = GetBlockFreeListEntry(prevHeader);
		}
		else if (prevHeader != nullptr)
		{
			// coalesce with prev block only, take its free-list entry for resulting block
			flEntryToReplace = GetBlockFreeListEntry(prevHeader);
		}
		else
		{
			// coalesce with next block only, take its free-list entry for resulting block
			flEntryToReplace = GetBlockFreeListEntry(nextHeader);
		}

		header = WriteBlockHeaderAndFooter(finalBlockStart, false, finalBlockSize);
		if (flEntryToReplace->PrevFreeHeader == flEntryToReplace->NextFreeHeader)
		{
			// block to coalesce with was the only free block, so resulting one will be the only one too
			PutInFreeList(header, header, header);
		}
		else
		{
			// block to coalesce with wasn't the only free block, so put resulting block on its position in free list
			PutInFreeList(header, flEntryToReplace->PrevFreeHeader, flEntryToReplace->NextFreeHeader);
		}
	}
	_cursor = header;
	
	return true;
}

void CoalesceAllocator::PutCursorOnFirstFitFreeBlock(short requiredSize)
{
	const BlockMeta* const start = _cursor;
	do
	{
		if (_cursor->Size >= requiredSize)
			return;

		auto blockFreeMeta = GetBlockFreeListEntry(_cursor);
		_cursor = blockFreeMeta->NextFreeHeader;
	} while (_cursor != start);

	// at this point, no suitable block is found, so allocate new page

	_page = InsertNewPage(_page);
	BlockMeta* blockHeader = WriteBlockHeaderAndFooter(_page->Start, false, _pageSize - sizeof(Page));
	PutInFreeList(blockHeader, _cursor, GetBlockFreeListEntry(_cursor)->NextFreeHeader);

	_cursor = reinterpret_cast<BlockMeta*>(_page->Start);
}


CoalesceAllocator::BlockMeta* CoalesceAllocator::WriteBlockHeaderAndFooter(char* start, bool isBusy, short blockSize)
{
	auto header = reinterpret_cast<BlockMeta*>(start);
	header->IsBusy = isBusy;
	header->Size = blockSize;

	auto footer = reinterpret_cast<BlockMeta*>(start + blockSize - sizeof(BlockMeta));
	footer->IsBusy = isBusy;
	footer->Size = blockSize;
	return header;
}


void CoalesceAllocator::PutInFreeList(BlockMeta* header, BlockMeta* prevFreeHeader, BlockMeta* nextFreeHeader)
{
	FreeListEntry* freeMeta = GetBlockFreeListEntry(header);

	freeMeta->PrevFreeHeader = prevFreeHeader;
	freeMeta->NextFreeHeader = nextFreeHeader;

	if (prevFreeHeader != header) // not looped on itself
	{
		GetBlockFreeListEntry(prevFreeHeader)->NextFreeHeader = header;
		GetBlockFreeListEntry(nextFreeHeader)->PrevFreeHeader = header;
	}
}

void CoalesceAllocator::RemoveFromFreeList(const FreeListEntry* entry)
{
	GetBlockFreeListEntry(entry->PrevFreeHeader)->NextFreeHeader = entry->NextFreeHeader;
	GetBlockFreeListEntry(entry->NextFreeHeader)->PrevFreeHeader = entry->PrevFreeHeader;
}


char* CoalesceAllocator::GetBlockContent(BlockMeta* header) const
{
	return reinterpret_cast<char*>(header) + sizeof(BlockMeta);
}

CoalesceAllocator::FreeListEntry* CoalesceAllocator::GetBlockFreeListEntry(BlockMeta* header) const
{
	return reinterpret_cast<FreeListEntry*>(GetBlockContent(header));
}


CoalesceAllocator::Page* CoalesceAllocator::InsertNewPage(Page* prevPage) const
{
	void* p = VirtualAlloc(nullptr, _pageSize, MEM_COMMIT, PAGE_READWRITE);
	if (p == nullptr)
		throw std::exception("Failed to receive a mem page");

	char* start = static_cast<char*>(p) + sizeof(Page);
	char* end = start + _pageSize - sizeof(Page);

	auto page = static_cast<Page*>(p);
	new(page) Page(start, end);

	if (prevPage == nullptr)
	{
		page->PrevPage = page;
		page->NextPage = page;
	}
	else
	{
		page->PrevPage = prevPage;
		page->NextPage = prevPage->NextPage;
		prevPage->NextPage->PrevPage = page;
		prevPage->NextPage = page;
	}
	return page;
}

bool CoalesceAllocator::IsPtrInPages(void* p)
{
	const Page* start = _page;
	do
	{
		if (_page->IsPtrInPage(p))
			return true;
		_page = _page->NextPage;
	} while (_page != start);
	return false;
}

bool CoalesceAllocator::Page::IsPtrInPage(const void* p) const
{
	return p >= Start && p < End;
}


#if _DEBUG

void CoalesceAllocator::DebugDumpFull() const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	using namespace std;

	cout << "[Coalesce Allocator] Dump full" << endl;

	Page* page = _page;
	do
	{
		cout << "Page " << static_cast<void*>(page->Start) << " - " << static_cast<void*>(page->End) << ";" << endl;

		int index = 0;
		char* rawCursor = _page->Start;
		while(rawCursor != _page->End)
		{
			auto header = reinterpret_cast<BlockMeta*>(rawCursor);
			cout << "Block: " << index << "  ";
			cout << static_cast<void*>(header) << "  ";
			cout << (header->IsBusy ? "b" : "f") << "  ";
			cout << "size: " << header->Size - sizeof(BlockMeta) * 2 << "(" << header->Size << ")  ";
			if (!header->IsBusy)
			{
				FreeListEntry* freeMeta = GetBlockFreeListEntry(header);
				cout << "prevFree: " << static_cast<void*>(freeMeta->PrevFreeHeader) << "  ";
				cout << "nextFree: " << static_cast<void*>(freeMeta->NextFreeHeader) << "  ";
			}
			cout << endl;
			index++;
			rawCursor += header->Size;
		}

		page = page->NextPage;
	} while (page != _page);
	cout << endl;
}

void CoalesceAllocator::DebugDumpStats(bool withLegend, int width) const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	_debugAllocStats.Dump(withLegend, width);
	std::cout << "Coalesce Allocator specifics, ";
	_debugAllocStats.DumpOnlyTotal();
}

void CoalesceAllocator::DebugDumpPages() const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	using namespace std;

	cout << "[Coalesce Allocator] Dump pages" << endl;

	int pagesCount = 0;
	Page* page = _page;
	do
	{
		cout << "Page " << static_cast<void*>(page->Start) << " - " << static_cast<void*>(page->End) << endl;
		page = page->NextPage;
		++pagesCount;
	} while (page != _page);

	cout << pagesCount << " pages, " << _pageSize << "b each, " << _pageSize * pagesCount << "b total" << endl;
}

void CoalesceAllocator::DebugDumpBusyBlocks() const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	using namespace std;

	Page* page = _page;
	do
	{
		char* rawCursor = page->Start;
		while (rawCursor != page->End)
		{
			auto header = reinterpret_cast<BlockMeta*>(rawCursor);
			if (header->IsBusy)
				cout << header->Size << "  " << static_cast<void*>(rawCursor) << endl;
			rawCursor += header->Size;
		}
		page = page->NextPage;
	} while (page != _page);
}

#endif