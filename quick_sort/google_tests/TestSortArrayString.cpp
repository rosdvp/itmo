#include <gtest/gtest.h>
#include <algorithm>
#include "../src/QuickSort.h"
#include "../src/TestHelpers.h"


TEST(SortArrayString, Manual)
{
	std::string arr[]{ "k", "a", "j", "b", "i", "c", "h", "d", "g", "e", "f"};
	Sort(arr, arr + 11, [](const std::string& a, const std::string& b) { return a < b; });

	if (!std::is_sorted(arr, arr + 11))
	{
		PrintArray("result", arr, 11);
		FAIL();
	}
}

TEST(SortArrayString, Randomized)
{
	srand(0);

	for (int count = 2; count < 20; count++)
		for (int attempt = 0; attempt < 100; attempt++)
		{
			auto arr = new std::string[count];
			FillStringArrayRandomly(arr, count, 3, 10);

			PrintArray("orig", arr, count);

			Sort(arr, arr + count, [](const std::string& a, const std::string& b) { return a < b; });

			if (!std::is_sorted(arr, arr + count))
			{
				PrintArray("sorted", arr, count);
				FAIL();
			}
			delete[] arr;
		}
};