#include "pch.h"
#include "TestHelpers.h"
#include "../itmo_quick_sort/QSort.h"
#include "../dynamic_array/Array.tpp"


TEST(SortDynamicArray, Empty)
{
	Array<int> arr;
	Sort(arr.begin(), arr.end(), [](const int& a, const int& b) { return a < b; });
}

TEST(SortDynamicArray, OneItem)
{
	Array<int> arr;
	arr.Insert(1);
	Sort(arr.begin(), arr.end(), [](const int& a, const int& b) { return a < b; });
	ASSERT_EQ(arr[0], 1);
}

TEST(SortDynamicArray, Randomized)
{
	srand(0);

	for (int count = 2; count < 20; count++)
		for (int attempt = 0; attempt < 100; attempt++)
		{
			Array<int> arr;
			for (int i = 0; i < count; i++)
				arr.Insert(rand() % 10);

			PrintArray("orig", arr.GetValuesPointer(), count);

			Sort(arr.begin(), arr.end(), [](const int& a, const int& b) { return a < b; });

			if (!std::is_sorted(arr.begin(), arr.end()))
			{
				PrintArray("orig", arr.GetValuesPointer(), count);
				FAIL();
			}
		}
};