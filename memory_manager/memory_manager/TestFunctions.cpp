#include <iostream>
#include <string>
#include <algorithm>

#include "TestFunctions.h"
#include "MemoryManager.h"


void TestManager()
{
	MemoryManager manager;
	manager.Init();

	void* a = manager.Alloc(4);
	void* b = manager.Alloc(4);
	void* c = manager.Alloc(8);
	void* d = manager.Alloc(8);
	void* e = manager.Alloc(16);
	void* f = manager.Alloc(16);
	void* g = manager.Alloc(24);
	void* h = manager.Alloc(24);
	void* i = manager.Alloc(32);
	void* j = manager.Alloc(32);

#if _DEBUG
	manager.DebugDumpStats(true, false);
	manager.DebugDumpBusyBlocks();
#endif

	manager.Free(a);
	manager.Free(b);
	manager.Free(c);
	manager.Free(d);
	manager.Free(e);
	manager.Free(f);
	manager.Free(g);
	manager.Free(h);
	manager.Free(i);
	manager.Free(j);

#if _DEBUG
	manager.DebugDumpStats(true, false);
#endif

	manager.Destroy();
}

void TestManagerDestroyWithBusyBlocks()
{
	MemoryManager manager;
	manager.Init();
	manager.Alloc(32);
	manager.Destroy();
}

void TestManagerIterative(int itersCount, size_t sizeMin, size_t sizeMax, int allocsInIterMin, int allocsInIterMax)
{
	srand(1);

	MemoryManager manager(sizeMax * 2, sizeMax * 2, sizeMax);
	manager.Init();

	for (int iterIdx = 0; iterIdx < itersCount; iterIdx++)
	{
		std::cout << "Iter #" << iterIdx << std::endl;

		std::vector<void*> ptrs;

		int allocsCount = allocsInIterMin + rand() % (allocsInIterMax - allocsInIterMin);
		for (int allocIdx = 0; allocIdx < allocsCount; allocIdx++)
		{
			size_t size = sizeMin + rand() % (sizeMax - sizeMin);

			std::cout << "Alloc " << size << std::endl;

			ptrs.push_back(manager.Alloc(size));
		}
#if _DEBUG
		manager.DebugDumpStats(false, false);
#endif
		std::cout << "###########################################" << std::endl;

		for (auto& ptr : ptrs)
			manager.Free(ptr);
		ptrs.clear();
	}
#if _DEBUG
	std::cout << "Final dump" << std::endl;
	manager.DebugDumpStats(true, true);
#endif

	manager.Destroy();
}


void TestManagerArrays(int itersCount, int allocsInIterMin, int allocsInIterMax, const std::vector<int>& arraysLengths)
{
	srand(1);

	size_t maxArraySize = *std::max_element(arraysLengths.begin(), arraysLengths.end()) * sizeof(int);
	std::cout << "Max Array Size (bytes): " << maxArraySize << std::endl;

	MemoryManager manager(maxArraySize + 1000, 0, maxArraySize);
	manager.Init();


	for (int iterIdx = 0; iterIdx < itersCount; iterIdx++)
	{
		std::cout << "Iter #" << iterIdx << std::endl;

		std::vector<void*> ptrs;

		std::map<int, std::vector<int*>*> arraysByLengths;
		for (int arrLength : arraysLengths)
			arraysByLengths[arrLength] = new std::vector<int*>();

		int allocsCount = allocsInIterMin + rand() % (allocsInIterMax - allocsInIterMin);
		for (int allocIdx = 0; allocIdx < allocsCount; allocIdx++)
		{
			int r = rand() % arraysLengths.size();
			int length = arraysLengths[r];
			size_t size = sizeof(int) * length;

			std::cout << "Alloc array(length: " << length << ", bytes: " << size << ")" << std::endl;

			auto arr = static_cast<int*>(manager.Alloc(size));
			int arrIdx = arraysByLengths[length]->size();
			for (int i = 0; i < length; i++)
				arr[i] = 1000 * arrIdx + i;

			ptrs.push_back(arr);
			arraysByLengths[length]->push_back(arr);
		}

		for (const auto& [length, arrays] : arraysByLengths)
			for (int arrIdx = 0; arrIdx < arrays->size(); arrIdx++)
			{
				int* arr = arrays->at(arrIdx);
				for (int i = 0; i < length; i++)
				{
					int expected = 1000 * arrIdx + i;
					if (arr[i] != expected)
						throw std::exception(("length: " + std::to_string(length) +
							", arrIdx: " + std::to_string(arrIdx) +
							", i: " + std::to_string(i) +
							", expected: " + std::to_string(expected) +
							", actual: " + std::to_string(arr[i])).c_str());
				}
			}
#if _DEBUG
		manager.DebugDumpStats(false, true);
#endif

		for (auto& p : ptrs)
			manager.Free(p);

		std::cout << "###########################################" << std::endl;

		for (int arrLength : arraysLengths)
			delete arraysByLengths[arrLength];
	}
#if _DEBUG
	manager.DebugDumpStats(true, true);
#endif
	manager.Destroy();
}


void TestFSA()
{
	FSAllocator allocator(80, 80, 16);
	allocator.Init();
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	void* a = allocator.Alloc();
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	void* b = allocator.Alloc();
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	void* c = allocator.Alloc();
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.TryFree(b);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.TryFree(a);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.TryFree(c);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.Destroy();
}


void TestCoalesceAllocator()
{
	CoalesceAllocator allocator(512, 512);
	allocator.Init();
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	auto a = allocator.Alloc(128);
#if _DEBUG
	allocator.DebugDumpFull();
#endif
	
	auto b = allocator.Alloc(64);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.TryFree(a);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	auto c = allocator.Alloc(16);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.TryFree(b);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.TryFree(c);
#if _DEBUG
	allocator.DebugDumpFull();
#endif

	allocator.Destroy();
}