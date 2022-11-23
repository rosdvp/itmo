#pragma once
#include <vector>

/**
 * \brief Simple test MemoryManager
 */
void TestManager();
/**
 * \brief Should assert in debug, and not assert in release.
 */
void TestManagerDestroyWithBusyBlocks();
/**
 * \brief On each iteration, do [allocsInIterMin; allocsInIterMax) allocations. 
 * Each allocation is a size of [sizeMin; sizeMax). 
 * Then dump stats, free all memory and start new iteration. 
 * At the end, dump full memory stats.
 */
void TestManagerIterative(int itersCount, size_t sizeMin, size_t sizeMax, int allocsInIterMin, int allocsInIterMax);
/**
 * \brief On each iteration, do [allocsInIterMin; allocsInIterMax) allocations of array.
 * Array length for each allocation is taken randomly from provided vector.
 * Then dump stats, free all memory and start new iteration.
 * At the end, dump full memory stats.
 */
void TestManagerArrays(int itersCount, int allocsInIterMin, int allocsInIterMax, const std::vector<int>& arraysLengths);
/**
 * \brief Simple test FSA
 */
void TestFSA();
/**
 * \brief Simple test CoalesceAllocator
 */
void TestCoalesceAllocator();

/**
 * \brief Simple test MemoryManager in release build
 */
void TestManagerInRelease();