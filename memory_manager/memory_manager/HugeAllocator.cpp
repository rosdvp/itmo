#include "HugeAllocator.h"

#include <crtdbg.h>
#include <Windows.h>

HugeAllocator::HugeAllocator()
#if _DEBUG
	:
	_isInitialized(),
	_currAllocatedCount(0)
#endif
{}

HugeAllocator::~HugeAllocator()
{
	_ASSERT_EXPR(_isInitialized == false, "Allocator is not destroyed");
}

void HugeAllocator::Init()
{
	_ASSERT_EXPR(_isInitialized == false, "Allocator is already initialized");

#if _DEBUG
	_isInitialized = true;
#endif
}

void HugeAllocator::Destroy()
{
	_ASSERT_EXPR(_isInitialized == true, "Allocator is not initialized");
	_ASSERT_EXPR(_currAllocatedCount == 0, "Not all memory are free");

#if _DEBUG
	_isInitialized = false;
#endif
}

void* HugeAllocator::Alloc(size_t size)
{
	_ASSERT_EXPR(_isInitialized == true, "Allocator is not initialized");
#if _DEBUG
	++_currAllocatedCount;
#endif
	return VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
}

void HugeAllocator::Free(void* p)
{
	_ASSERT_EXPR(_isInitialized == true, "Allocator is not initialized");
#if _DEBUG
	--_currAllocatedCount;
#endif
	VirtualFree(p, 0, MEM_RELEASE);
}