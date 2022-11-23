#pragma once
#include <map>
#include <vector>

#if _DEBUG

class DebugUsageRecorder
{
public:

	void RecordAlloc(void* p, size_t size);

	void RecordFree(void* p);

	void Clear();

	void Dump() const;

private:
	std::map<size_t, int> _currAllocatedCountBySize;

	std::map<size_t, int> _peakAllocatedCountBySize;

	std::map<size_t, int> _allocsCountBySize;
	std::map<size_t, int> _freesCountBySize;

	std::map<void*, size_t> _sizesMap;
};

#endif