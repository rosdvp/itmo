#include "DebugUsageRecorder.h"

#include <iomanip>
#include <iostream>

#if _DEBUG

void DebugUsageRecorder::RecordAlloc(void* p, size_t size)
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

	_sizesMap[p] = size;
}

void DebugUsageRecorder::RecordFree(void* p)
{
	size_t size = _sizesMap[p];

	_allocsCountBySize.emplace(size, 0);
	_freesCountBySize.emplace(size, 0);
	_currAllocatedCountBySize.emplace(size, 0);
	_peakAllocatedCountBySize.emplace(size, 0);

	++_freesCountBySize[size];

	--_currAllocatedCountBySize[size];
}

void DebugUsageRecorder::Clear()
{
	_allocsCountBySize.clear();
	_freesCountBySize.clear();
	_currAllocatedCountBySize.clear();
	_peakAllocatedCountBySize.clear();
}

void DebugUsageRecorder::Dump() const
{
	using namespace std;

	int width = 20;
	cout << setw(width) << left << "Requested Size";
	cout << setw(width) << left << "Allocated Now";
	cout << setw(width) << left << "Peak Allocated";
	cout << setw(width) << left << "Allocs Total";
	cout << setw(width) << left << "Frees Total";
	cout << endl;

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
#endif