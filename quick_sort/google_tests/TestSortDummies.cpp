#include <gtest/gtest.h>
#include <algorithm>
#include "../src/QuickSort.h"
#include "../src/TestHelpers.h"
#include "Dummy.h"


TEST(SortDummies, Randomized)
{
	srand(0);

	auto funLessThan = [](const Dummy& a, const Dummy& b)
	{
		return a.Power < b.Power || a.Power == b.Power && a.Value < b.Value;
	};
	auto funLessThanOrEqual = [](const Dummy& a, const Dummy& b)
	{
		return a.Power < b.Power || a.Power == b.Power && a.Value <= b.Value;
	};

	for (int count = 2; count < 20; count++)
		for (int attempt = 0; attempt < 100; attempt++)
		{
			const auto arr = static_cast<Dummy*>(malloc(sizeof(Dummy) * count));
			FillArrayRandomly(arr, count, [] { return Dummy(rand() % 10, rand() % 10); });

			PrintArray("orig", arr, count);

			Sort(arr, arr + count, funLessThan);

			auto isSorted = true;
			for (int i = 0; i < count-1; i++)
				if (!funLessThanOrEqual(arr[i], arr[i+1]))
				{
					isSorted = false;
					break;
				}

			if (!isSorted)
			{
				PrintArray("result", arr, count);
				FAIL();
			}
			free(arr);
		}
};