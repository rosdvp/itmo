#pragma once
#include <set>

#include "DebugAllocStats.h"

class FSAllocator
{
public:
	/**
	 * \brief pageSize will be aligned up to osPageSize.
	 * \param allocatorPageSize Allocator page might include multiple OS virtual pages.
	 * \param osPageSize Operating system page size.
	 */
	FSAllocator(size_t osPageSize, size_t allocatorPageSize, size_t blockSize);
	/**
	 * \brief pageSize will be aligned up to OS virtual page size taken from WIN API.
	 * \param allocatorPageSize Allocator page might include multiple OS virtual pages.
	 */
	FSAllocator(size_t allocatorPageSize, size_t blockSize);
	~FSAllocator();

	FSAllocator() = delete;

	void Init();
	void Destroy();

	void* Alloc();
	bool TryFree(void* p);

private:
	const size_t _pageSize;
	const size_t _blockSize;

	struct Page;
	Page* _page; // Pointer to current page

	/**
	 * \brief Pages are in looped linked list.
	 * \param prevPage new page will be inserted after this one. If null, loops on itself.
	 */
	Page* InsertNewPage(Page* prevPage) const;

	Page* FindNotFullPage() const;
	Page* FindPageOfBlock(const char* block) const;
	/**
	 * \brief Relative to current page (_page)
	 */
	char* IdxToBlock(int idx) const;
	char* IdxToBlock(const Page* page, int idx) const;
	/**
	 * \brief Relative to current page (_page)
	 */
	int BlockToIdx(const char* block) const;

	struct Page
	{
		int InitializedBlocksCount;
		char* const Start;
		char* const End;
		char* Cursor;		//Always point on free block in this page, except when page is full
		Page* PrevPage;
		Page* NextPage;

		Page(char* start, char* end)
			: InitializedBlocksCount(1),
			Start(start), End(end), Cursor(start),
			PrevPage(nullptr), NextPage(nullptr)
		{}

		bool IsBlockInPage(const char* block) const;
		/**
		 * \return True if no blocks left in free list and everything was initialized.
		 */
		bool IsFull() const;
	};
	
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
	 * \brief COUT all busy blocks, their sizes and addresses.
	 */
	void DebugDumpBusyBlocks() const;

private:
	DebugAllocStats _debugAllocStats;

	std::set<char*> DebugFindAllFreeBlocks() const;
#endif
};
