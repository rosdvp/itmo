#pragma once
#include "CoalesceAllocator.h"
#include "DebugAllocStats.h"

class CoalesceAllocator
{
public:
	/**
	 * \brief pageSize will be aligned up to osPageSize. 
	 * Take into account, that max requested block size should be less than pageSize.
	 * \param allocatorPageSize Allocator page might include multiple OS virtual pages.
	 * \param osPageSize Operating system page size.
	 */
	CoalesceAllocator(size_t osPageSize, size_t allocatorPageSize);
	/**
	 * \brief pageSize will be aligned up to OS virtual page size taken from WIN API. 
	 * Take into account, that max requested block size should be less than pageSize.
	 * \param allocatorPageSize Allocator page might include multiple OS virtual pages.
	 */
	CoalesceAllocator(size_t allocatorPageSize);
	~CoalesceAllocator();

	CoalesceAllocator() = delete;

	void Init();
	void Destroy();

	void* Alloc(short contentSize);
	bool TryFree(void* p);

private:
	struct Page;
	struct BlockMeta;
	struct FreeListEntry;

	const size_t _pageSize;
	
	Page* _page; // Pointer to pages' looped linked list; might not point on page of _cursor.
	BlockMeta* _cursor; // Always points on free block header.

	/**
	 * \brief Modifies _cursor to point on suitable block. If no such block found, allocates new page.
	 * \param requiredSize Including header + footer.
	 */
	void PutCursorOnFirstFitFreeBlock(short requiredSize);

	/**
	 * \param blockSize Including header + footer.
	 */
	BlockMeta* WriteBlockHeaderAndFooter(char* start, bool isBusy, short blockSize);

	/**
	 * \brief Add or replace block in looped free list. 
	 * If free list is empty, pass header to prev and next params to loop on itself. 
	 * \param prevFreeHeader Its NextFreeHeader will be replaced by header
	 * \param nextFreeHeader Its PrevFreeHeader will be replaced by header
	 */
	void PutInFreeList(BlockMeta* header, BlockMeta* prevFreeHeader, BlockMeta* nextFreeHeader);
	/**
	 * \brief Connect entry's prev and next between each other, so entry becomes out of list.
	 */
	void RemoveFromFreeList(const FreeListEntry* entry);

	char* GetBlockContent(BlockMeta* header) const;
	FreeListEntry* GetBlockFreeListEntry(BlockMeta* header) const;

	/**
	 * \brief Pages are in looped linked list.
	 * \param prevPage new page will be inserted after this one. If null, loops on itself.
	 */
	Page* InsertNewPage(Page* prevPage) const;
	/**
	 * \brief Scan all pages until ptr is not in (Start; End) range or all pages are scanned. 
	 * Modifies _page.
	 */
	bool IsPtrInPages(void* p);


	struct Page
	{
		char* const Start;
		char* const End;
		Page* PrevPage;
		Page* NextPage;

		Page(char* start, char* end)
			: Start(start), End(end),
			PrevPage(nullptr), NextPage(nullptr)
		{}

		bool IsPtrInPage(const void* p) const;
	};

	/**
	 * \brief Can be header or footer.
	 */
	struct BlockMeta
	{
		bool IsBusy;
		short Size; // including header + footer
		
		BlockMeta(bool isBusy, short size) : IsBusy(isBusy), Size(size) { }
	};

	/**
	 * \brief Struct is written instead of block's content if block is free. 
	 */
	struct FreeListEntry
	{
		BlockMeta* PrevFreeHeader;
		BlockMeta* NextFreeHeader;
	};

public:
	static constexpr short BLOCK_MIN_SIZE = sizeof(BlockMeta) * 2 + sizeof(FreeListEntry);
	static constexpr short PAGE_META_SIZE = sizeof(Page);

#if _DEBUG
public:
	/**
	 * \brief COUT all blocks and their full info.
	 */
	void DebugDumpFull() const;
	/**
	 * \brief COUT number of allocated blocks, peaks and alloc/free statistics.
	 * \param withLegend COUT legend at the beginning.
	 * \param width Count of symbols in each column (display setting).
	 */
	void DebugDumpStats(bool withLegend, int width) const;
	/**
	 * \brief COUT allocated pages.
	 */
	void DebugDumpPages() const;
	/**
	 * \brief COUT each busy block, its size and address.
	 */
	void DebugDumpBusyBlocks() const;
private:
	DebugAllocStats _debugAllocStats;
#endif
};
