#pragma once
#include <Windows.h>

inline size_t GetOSVirtualPageSize()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwPageSize;
}

inline size_t AlignSizeIncreasing(size_t size, size_t alignBy)
{
	return (size / alignBy + (size % alignBy > 0 ? 1 : 0)) * alignBy;
}

inline size_t AlignSizeDecreasing(size_t size, size_t alignBy)
{
	return (size / alignBy) * alignBy;
}