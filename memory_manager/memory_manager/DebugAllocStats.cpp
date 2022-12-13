#include "DebugAllocStats.h"

#include <iomanip>
#include <iostream>

#if _DEBUG

void DebugAllocStats::RecordAlloc(size_t size)
{
	_allocsCountBySize.emplace(size, 0);
	_freesCountBySize.emplace(size, 0);
	_currAllocatedCountBySize.emplace(size, 0);
	_peakAllocatedCountBySize.emplace(size, 0);

	++_allocsCountBySize[size];

	int count = _currAllocatedCountBySize[size] + 1;
	_currAllocatedCountBySize[size] = count;

	if (count > _peakAllocatedCountBySize[size])
		_peakAllocatedCountBySize[size] = count;

	_currAllocatedSize += size;
	if (_currAllocatedSize > _peakAllocatedSize)
		_peakAllocatedSize = _currAllocatedSize;
}

void DebugAllocStats::RecordFree(size_t size)
{
	_allocsCountBySize.emplace(size, 0);
	_freesCountBySize.emplace(size, 0);
	_currAllocatedCountBySize.emplace(size, 0);
	_peakAllocatedCountBySize.emplace(size, 0);

	++_freesCountBySize[size];

	--_currAllocatedCountBySize[size];

	_currAllocatedSize -= size;
}

void DebugAllocStats::Clear()
{
	_allocsCountBySize.clear();
	_freesCountBySize.clear();
	_currAllocatedCountBySize.clear();
	_peakAllocatedCountBySize.clear();
}

bool DebugAllocStats::IsAnyAllocatedNow() const
{
	for (const auto& [size, currAllocatedCount] : _currAllocatedCountBySize)
		if (currAllocatedCount != 0)
			return true;
	return false;
}

void DebugAllocStats::Dump(bool withLegend, int width) const
{
	using namespace std;

	if (withLegend)
	{
		cout << setw(width) << left << "Block Size";
		cout << setw(width) << left << "Allocated Now";
		cout << setw(width) << left << "Peak Allocated";
		cout << setw(width) << left << "Allocs Total";
		cout << setw(width) << left << "Frees Total";
		cout << endl;
	}

	for (const auto& [size, currAllocatedCount] : _currAllocatedCountBySize)
	{
		cout << setw(width) << left << size;
		cout << setw(width) << left << currAllocatedCount;
		cout << setw(width) << left << _peakAllocatedCountBySize.at(size);
		cout << setw(width) << left << _allocsCountBySize.at(size);
		cout << setw(width) << left << _freesCountBySize.at(size);
		cout << endl;
	}
}

void DebugAllocStats::DumpOnlyTotal() const
{
	using namespace std;
	cout << "Allocated Size Now: " << _currAllocatedSize << "  ";
	cout << "Allocated Size Peak: " << _peakAllocatedSize;
	cout << endl;
}

#endif