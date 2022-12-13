#include <iostream>

#include "MemoryManager.h"
#include "AllocatorsHelpers.h"


MemoryManager::MemoryManager(size_t osPageSize, size_t allocatorsPageSize, size_t maxSizeForCA) :
	_osPageSize(osPageSize == 0 ? GetOSVirtualPageSize() : osPageSize),
	_allocatorPageSize(allocatorsPageSize == 0 ? _osPageSize : allocatorsPageSize),
	_maxSizeForCA(maxSizeForCA == 0 ? DEFAULT_MAX_SIZE_FOR_CA : maxSizeForCA),
	_fsa16(_osPageSize, _allocatorPageSize, 16),
	_fsa32(_osPageSize, _allocatorPageSize, 32),
	_fsa64(_osPageSize, _allocatorPageSize, 64),
	_fsa128(_osPageSize, _allocatorPageSize, 128),
	_fsa256(_osPageSize, _allocatorPageSize, 256),
	_fsa512(_osPageSize, _allocatorPageSize, 512),
	_ca(_osPageSize, AlignSizeIncreasing(_allocatorPageSize,
		_maxSizeForCA + CoalesceAllocator::PAGE_META_SIZE))
#if _DEBUG
	, _isInitialized(false)
#endif
{}

MemoryManager::~MemoryManager()
{
	_ASSERT_EXPR(!_isInitialized, "Manager should be destroyed first");
}

void MemoryManager::Init()
{
	_ASSERT_EXPR(!_isInitialized, "Manager is already initiailized");

	_fsa16.Init();
	_fsa32.Init();
	_fsa64.Init();
	_fsa128.Init();
	_fsa256.Init();
	_fsa512.Init();
	_ca.Init();
	_ha.Init();
#if _DEBUG
	_isInitialized = true;
#endif
}

void MemoryManager::Destroy()
{
	_ASSERT_EXPR(_isInitialized, "Manager is not initialized");

	_fsa16.Destroy();
	_fsa32.Destroy();
	_fsa64.Destroy();
	_fsa128.Destroy();
	_fsa256.Destroy();
	_fsa512.Destroy();
	_ca.Destroy();
	_ha.Destroy();
#if _DEBUG
	_isInitialized = false;
#endif
#if MEM_MANAGER_DEBUG_USAGE
	_debugUsageRecorder.Clear();
#endif
}

void* MemoryManager::Alloc(size_t size)
{
	_ASSERT_EXPR(_isInitialized, "Manager is not initialized");

	void* p;
	if (size <= 16)
		p = _fsa16.Alloc();
	else if (size <= 32)
		p = _fsa32.Alloc();
	else if (size <= 64)
		p = _fsa64.Alloc();
	else if (size <= 128)
		p = _fsa128.Alloc();
	else if (size <= 256)
		p = _fsa256.Alloc();
	else if (size <= 512)
		p = _fsa512.Alloc();
	else if (size <= DEFAULT_MAX_SIZE_FOR_CA)
		p = _ca.Alloc(size);
	else
		p = _ha.Alloc(size);

#if MEM_MANAGER_DEBUG_USAGE
	_debugUsageRecorder.RecordAlloc(p, size);
#endif

	return p;
}

void MemoryManager::Free(void* p)
{
	_ASSERT_EXPR(_isInitialized, "Manager is not initialized");
#if MEM_MANAGER_DEBUG_USAGE
	_debugUsageRecorder.RecordFree(p);
#endif

	if (!_fsa16.TryFree(p) &&
		!_fsa32.TryFree(p) &&
		!_fsa64.TryFree(p) &&
		!_fsa128.TryFree(p) &&
		!_fsa256.TryFree(p) &&
		!_fsa512.TryFree(p) &&
		!_ca.TryFree(p))
	{
		_ha.Free(p);
	}
}

#if _DEBUG

void MemoryManager::DebugDumpFull() const
{
	_ASSERT_EXPR(_isInitialized, "Manager is not initialized");
	using namespace std;
	_fsa16.DebugDumpFull();
	_fsa32.DebugDumpFull();
	_fsa64.DebugDumpFull();
	_fsa128.DebugDumpFull();
	_fsa256.DebugDumpFull();
	_fsa512.DebugDumpFull();
	_ca.DebugDumpFull();
}

void MemoryManager::DebugDumpStats(bool withManagerUsage, bool withPages) const
{
	_ASSERT_EXPR(_isInitialized, "Manager is not initialized");
	using namespace std;

	if (withManagerUsage)
	{
#if MEM_MANAGER_DEBUG_USAGE
		cout << "[Memory Manager] Dump usage (precise requests to manager, not aligned by allocators)" << endl;
		_debugUsageRecorder.Dump();
#else
		cout << "[Manager Manager] Dump usage is unavailable, turn on macro MAM_MANAGER_DEBUG_USAGE in source file." << endl;
		cout << "(will decrease performance!)" << endl;
#endif
		cout << "---------------" << endl;
	}

	cout << "[Memory Manager] Dump stats (block size aligned by allocators)" << endl;
	int width = 20;
	_fsa16.DebugDumpStats(true, width);
	_fsa32.DebugDumpStats(false, width);
	_fsa64.DebugDumpStats(false, width);
	_fsa128.DebugDumpStats(false, width);
	_fsa256.DebugDumpStats(false, width);
	_fsa512.DebugDumpStats(false, width);
	_ca.DebugDumpStats(false, width);

	if (withPages)
	{
		cout << "---------------" << endl;
		_fsa16.DebugDumpPages();
		cout << "---------------" << endl;
		_fsa32.DebugDumpPages();
		cout << "---------------" << endl;
		_fsa64.DebugDumpPages();
		cout << "---------------" << endl;
		_fsa128.DebugDumpPages();
		cout << "---------------" << endl;
		_fsa256.DebugDumpPages();
		cout << "---------------" << endl;
		_fsa512.DebugDumpPages();
		cout << "---------------" << endl;
		_ca.DebugDumpPages();
	}
	cout << "----------------------------------------" << endl;
}

void MemoryManager::DebugDumpBusyBlocks() const
{
	_ASSERT_EXPR(_isInitialized, "Manager is not initialized");
	using namespace std;

	cout << "[Memory Manager] Dump busy blocks" << endl;
	cout << "Size  Address" << endl;

	_fsa16.DebugDumpBusyBlocks();
	_fsa32.DebugDumpBusyBlocks();
	_fsa64.DebugDumpBusyBlocks();
	_fsa128.DebugDumpBusyBlocks();
	_fsa256.DebugDumpBusyBlocks();
	_fsa512.DebugDumpBusyBlocks();
	_ca.DebugDumpBusyBlocks();

	cout << "----------------------------------------" << endl;
}
#endif