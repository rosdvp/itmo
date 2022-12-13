#include <exception>
#include <iostream>

#include "FSAllocator.h"
#include "AllocatorsHelpers.h"
#include "Windows.h"

FSAllocator::FSAllocator(size_t osPageSize, size_t allocatorPageSize, size_t blockSize) :
	_pageSize(AlignSizeIncreasing(allocatorPageSize, osPageSize)),
	_blockSize(blockSize),
	_page(nullptr)
{
}

FSAllocator::FSAllocator(size_t allocatorPageSize, size_t blockSize) :
	FSAllocator(GetOSVirtualPageSize(), allocatorPageSize, blockSize)
{}

FSAllocator::~FSAllocator()
{
	_ASSERT_EXPR(_page == nullptr, "Allocator is not destroyed");
}

void FSAllocator::Init()
{
	_ASSERT_EXPR(_page == nullptr, "Allocator is already initialized");

	_page = InsertNewPage(nullptr);
}

void FSAllocator::Destroy()
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

void* FSAllocator::Alloc()
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");

	void* block = _page->Cursor;

	// cursor should always be on free block, do it.
	int nextFreeIdx = *static_cast<int*>(block);
	if (nextFreeIdx == -1) // nothing left in free list, try initialize
	{
		_page->Cursor = IdxToBlock(_page->InitializedBlocksCount);
		if (_page->IsFull()) // no left for initialization, try other pages
		{
			Page* notFullPage = FindNotFullPage();
			if (notFullPage == nullptr) // no pages with free list or initialize space left
				_page = InsertNewPage(_page->PrevPage);
			else
				_page = notFullPage; // notFullPage cursor is always on free block
		}
		else
		{
			*reinterpret_cast<int*>(_page->Cursor) = -1; // mark, that nothing left in free list
			_page->InitializedBlocksCount += 1;
		}
	}
	else // take from free list
	{
		_page->Cursor = _page->Start + nextFreeIdx * _blockSize;
	}

#if _DEBUG
	_debugAllocStats.RecordAlloc(_blockSize);
#endif

	return block;
}

bool FSAllocator::TryFree(void* p)
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");

	auto block = static_cast<char*>(p);

	if (!_page->IsBlockInPage(block))
	{
		Page* pageWithBlock = FindPageOfBlock(block);
		if (pageWithBlock == nullptr)
			return false;
		_page = pageWithBlock;
	}
	*reinterpret_cast<int*>(block) = _page->IsFull()
		? -1	// page was full, no free list here
		: BlockToIdx(_page->Cursor);	// add prev cursor's block to free list (since it must be free for sure)
	_page->Cursor = block;

#if _DEBUG
	_debugAllocStats.RecordFree(_blockSize);
#endif

	return true;
}

FSAllocator::Page* FSAllocator::InsertNewPage(Page* prevPage) const
{
	void* p = VirtualAlloc(nullptr, _pageSize, MEM_COMMIT, PAGE_READWRITE);
	if (p == nullptr)
		throw std::exception("Failed to receive a mem page");

	char* start = static_cast<char*>(p) + sizeof(Page);
	char* end = start + AlignSizeDecreasing(_pageSize - sizeof(Page), _blockSize);

	auto page = static_cast<Page*>(p);
	new(page) Page(start, end);

	*reinterpret_cast<int*>(page->Cursor) = -1;

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


FSAllocator::Page* FSAllocator::FindNotFullPage() const
{
	Page* selected = _page;
	do
	{
		selected = selected->NextPage;
		if (!selected->IsFull())
			return selected;
	} while (_page != selected);
	return nullptr;
}


FSAllocator::Page* FSAllocator::FindPageOfBlock(const char* block) const
{
	Page* selected = _page;
	do
	{
		selected = selected->NextPage;
		if (selected->IsBlockInPage(block))
			return selected;
	}
	while (_page != selected);
	return nullptr;
}

char* FSAllocator::IdxToBlock(const int idx) const
{
	return _page->Start + idx * _blockSize;
}

char* FSAllocator::IdxToBlock(const Page* page, const int idx) const
{
	return page->Start + idx * _blockSize;
}

int FSAllocator::BlockToIdx(const char* block) const
{
	return (block - _page->Start) / _blockSize;
}


bool FSAllocator::Page::IsBlockInPage(const char* block) const
{
	return block >= Start && block < End;
}

bool FSAllocator::Page::IsFull() const
{
	return Cursor == End;
}


#if _DEBUG

void FSAllocator::DebugDumpFull() const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	using namespace std;
	
	cout << "[FSA " << _blockSize << "] Dump full" << endl;

	set<char*> freeBlocks = DebugFindAllFreeBlocks();

	Page* page = _page;
	int blocksPerPage = (_page->End - _page->Start) / _blockSize;
	do
	{
		auto rawPtrPage = reinterpret_cast<char*>(page);
		cout << "Page " << static_cast<void*>(rawPtrPage) << " - " << static_cast<void*>(rawPtrPage + _pageSize) << ";" << endl;
		
		for (int i = 0; i < blocksPerPage; i++)
		{
			char* block        = IdxToBlock(page, i);
			bool isInitialized = i < page->InitializedBlocksCount;
			bool isFree        = freeBlocks.find(block) != freeBlocks.end();

			cout << "Block " << i << "  ";
			cout << static_cast<void*>(block) << "  ";
			cout << (isInitialized ? isFree ? 'f' : 'b' : 'n') << "  ";
			cout << "nextFreeIdx: ";
			if (isFree)
				cout << *reinterpret_cast<int*>(block);
			cout << endl;
		}
		cout << "----------" << endl;

		page = page->NextPage;
	} while (page != _page);
	cout << endl;
}

void FSAllocator::DebugDumpStats(bool withLegend, int width) const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	_debugAllocStats.Dump(withLegend, width);
}

void FSAllocator::DebugDumpPages() const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	using namespace std;

	cout << "[FSA " << _blockSize << "] Dump pages" << endl;

	int pagesCount = 0;
	Page* page = _page;
	do
	{
		auto rawPtrPage = reinterpret_cast<char*>(page);
		cout << "Page " << static_cast<void*>(rawPtrPage) << " - " << static_cast<void*>(rawPtrPage + _pageSize) << endl;
		page = page->NextPage;
		++pagesCount;
	} while (page != _page);

	cout << pagesCount << " pages, " << _pageSize << "b each, " << _pageSize * pagesCount << "b total" << endl;
}

void FSAllocator::DebugDumpBusyBlocks() const
{
	_ASSERT_EXPR(_page != nullptr, "Allocator is not initialized");
	using namespace std;

	set<char*> freeBlocks = DebugFindAllFreeBlocks();

	Page* page        = _page;
	int blocksPerPage = (_page->End - _page->Start) / _blockSize;
	do
	{
		for (int i = 0; i < blocksPerPage; i++)
		{
			char* block        = IdxToBlock(page, i);
			bool isInitialized = i < page->InitializedBlocksCount;
			bool isFree        = freeBlocks.find(block) != freeBlocks.end();

			if (!isInitialized || isFree)
				continue;
				
			cout << _blockSize << "  " << static_cast<void*>(block) << endl;
		}
		page = page->NextPage;
	} while (page != _page);
}

std::set<char*> FSAllocator::DebugFindAllFreeBlocks() const
{
	std::set<char*> freeBlocks;

	Page* page = _page;
	do
	{
		if (!page->IsFull())
		{
			char* nextFreeBlock = page->Cursor;
			do
			{
				freeBlocks.insert(nextFreeBlock);
				int nextFreeIdx = *reinterpret_cast<int*>(nextFreeBlock);
				nextFreeBlock = nextFreeIdx == -1 ? nullptr : IdxToBlock(page, nextFreeIdx);
			} while (nextFreeBlock != nullptr);
		}
		page = page->NextPage;
	} while (page != _page);

	return freeBlocks;
}

#endif