#include <gtest/gtest.h>
#include <algorithm>
#include "../src/QuickSort.h"
#include "../src/TestHelpers.h"


TEST(SortVectorInt, Manual)
{
	std::vector<int> vect{ 10, 11, 9, 12, 8, 13, 7, 14, 6, 15, 5, 16, 4, 17, 3, 18, 2, 19, 1, 20 };
	Sort(vect.begin(), vect.end(), [](const int& a, const int& b) { return a < b; });

	if (!std::is_sorted(vect.begin(), vect.end()))
	{
		PrintArray("result", vect.data(), 20);
		FAIL();
	}
}

TEST(SortVectorInt, Randomized)
{
	srand(0);

	for (int count = 2; count < 20; count++)
		for (int attempt = 0; attempt < 100; attempt++)
		{
			std::vector<int> vect(count, 0);
			FillIntArrayRandomly(vect.data(), count);
			PrintArray("orig", vect.data(), count);

			Sort(vect.begin(), vect.end(), [](const int& a, const int& b) { return a < b; });

			if (!std::is_sorted(vect.begin(), vect.end()))
			{
				PrintArray("sorted", vect.data(), count);
				FAIL();
			}
		}
};