#include <gtest/gtest.h>
#include <algorithm>
#include "../src/QuickSort.h"
#include "../src/TestHelpers.h"


TEST(SortArrayInt, Empty)
{
	int* arr = new int[0];
	Sort(arr, arr, [](const int& a, const int& b) { return a < b; });
	delete[] arr;
}

TEST(SortArrayInt, OneItem)
{
	int arr[1] { 1 };
	Sort(arr, arr+1, [](const int& a, const int& b) { return a < b; });
	ASSERT_EQ(arr[0], 1);
}

TEST(SortArrayInt, Manual)
{
	int arr[20] { 10, 11, 9, 12, 8, 13, 7, 14, 6, 15, 5, 16, 4, 17, 3, 18, 2, 19, 1, 20 };
	Sort(arr, arr + 20, [](const int& a, const int& b) { return a < b; });

	if (!std::is_sorted(arr, arr+20))
	{
		PrintArray("result", arr, 20);
		FAIL();
	}
}

TEST(SortArrayInt, Randomized)
{
	srand(0);
	
	for (int count = 2; count < 20; count++)
		for (int attempt = 0; attempt < 100; attempt++)
		{
			auto arr = new int[count];
			FillIntArrayRandomly(arr, count);

			PrintArray("orig", arr, count);

			Sort(arr, arr + count, [](const int& a, const int& b) { return a < b; });

			if (!std::is_sorted(arr, arr + count))
			{
				PrintArray("sorted", arr, count);
				FAIL();
			}

			delete[] arr;
		}
};