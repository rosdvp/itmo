#pragma once
#include <map>

#if _DEBUG
class DebugAllocStats
{
public:
	void RecordAlloc(size_t size);

	void RecordFree(size_t size);

	void Clear();

	bool IsAnyAllocatedNow() const;

	void Dump(bool withLegend, int width) const;

	void DumpOnlyTotal() const;

private:
	size_t _currAllocatedSize = 0;
	size_t _peakAllocatedSize = 0;

	std::map<size_t, int> _currAllocatedCountBySize;
	std::map<size_t, int> _peakAllocatedCountBySize;
	std::map<size_t, int> _allocsCountBySize;
	std::map<size_t, int> _freesCountBySize;
};
#endif