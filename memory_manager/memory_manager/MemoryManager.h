#pragma once

#include "CoalesceAllocator.h"
#include "DebugUsageRecorder.h"
#include "FSAllocator.h"
#include "HugeAllocator.h"

/**
 * \brief Turn recording exact usage of memory manager.
 * DebugDumpStats(true..) will show exact size of requested block.
 * Useful to define if new FSA is needed for specific size.
 * Poor performance!
 */
#define MEM_MANAGER_DEBUG_USAGE _DEBUG && 1


class MemoryManager
{
public:
	/**
	 * \brief pageSize will be aligned up to osPageSize. 
	 * For CoalesceAllocator, pageSize additionally will be aligned up to maxSizeForCA + its page meta size.
	 * \param osPageSize Operating system virtual page size. 
	 * If 0, taken from WIN API (typically 4096B).
	 * \param allocatorsPageSize Allocators page size might include multiple OS virtual pages. 
	 * Do that to optimize search on free operation, if you sure that OS page is not enough for entire program.
	 * If 0, is equal to osPageSize.
	 * \param maxSizeForCA If requested size is greater than this value, allocation will happen via VirtualAlloc.
	 * If 0, is equal to 10MB.
	 */
	MemoryManager(size_t osPageSize = 0, size_t allocatorsPageSize = 0, size_t maxSizeForCA = 0);

	~MemoryManager();

	void Init();
	void Destroy();

	void* Alloc(size_t size);
	void Free(void* p);

private:
	static constexpr size_t DEFAULT_MAX_SIZE_FOR_CA = 10240;

	const size_t _osPageSize;
	const size_t _allocatorPageSize;
	const size_t _maxSizeForCA;

	FSAllocator _fsa16;
	FSAllocator _fsa32;
	FSAllocator _fsa64;
	FSAllocator _fsa128;
	FSAllocator _fsa256;
	FSAllocator _fsa512;
	CoalesceAllocator _ca;
	HugeAllocator _ha;

#if _DEBUG
public:
	void DebugDumpFull() const;
	void DebugDumpStats(bool withManagerUsage, bool withPages) const;
	void DebugDumpBusyBlocks() const;

	const FSAllocator& DebugGetFSA16ForDumping() const { return _fsa16; }
	const FSAllocator& DebugGetFSA32ForDumping() const { return _fsa32; }
	const FSAllocator& DebugGetFSA64ForDumping() const { return _fsa64; }
	const FSAllocator& DebugGetFSA128ForDumping() const { return _fsa128; }
	const FSAllocator& DebugGetFSA256ForDumping() const { return _fsa256; }
	const FSAllocator& DebugGetFSA512ForDumping() const { return _fsa512; }
	const CoalesceAllocator& DebugGetCoalesceAllocatorForDumping() const { return _ca; }
private:
	bool _isInitialized;
#if MEM_MANAGER_DEBUG_USAGE
	DebugUsageRecorder _debugUsageRecorder;
#endif
#endif
};
