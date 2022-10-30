#include "pch.h"
#include "../dynamic_array/Array.h"

TEST(Int, CreateEmpty)
{
	Array<int> arr;
	ASSERT_EQ(arr.Size(), 0);
}

TEST(Int, InsertAtEnd)
{
	Array<int> arr;
	arr.Insert(0);
	ASSERT_EQ(arr.Size(), 1);
	arr.Insert(1);
	ASSERT_EQ(arr.Size(), 2);
	arr.Insert(2);
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 1);
	ASSERT_EQ(arr[2], 2);
}

TEST(Int, InsertAtBegin)
{
	Array<int> arr;
	arr.Insert(0, 2);
	ASSERT_EQ(arr.Size(), 1);
	arr.Insert(0, 1);
	ASSERT_EQ(arr.Size(), 2);
	arr.Insert(0, 0);
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 1);
	ASSERT_EQ(arr[2], 2);
}

TEST(Int, InsertAtMiddle)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(3);
	arr.Insert(4);
	arr.Insert(2, 2);
	ASSERT_EQ(arr.Size(), 5);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 1);
	ASSERT_EQ(arr[2], 2);
	ASSERT_EQ(arr[3], 3);
	ASSERT_EQ(arr[4], 4);
}

TEST(Int, IncreaseCapacity)
{
	Array<int> arr;

	const int initCapacity = arr.GetCapacity();
	for (int i = 0; i < 100; i++)
		arr.Insert(i);

	ASSERT_LT(initCapacity, arr.GetCapacity());
	ASSERT_EQ(arr.Size(), 100);
	for (int i = 0; i < 100; i++)
		ASSERT_EQ(arr[i], i);
}

TEST(Int, RemoveFromEnd)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	arr.Remove(2);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 1);

	arr.Remove(1);
	ASSERT_EQ(arr.Size(), 1);
	ASSERT_EQ(arr[0], 0);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 0);
}

TEST(Int, RemoveFromBegin)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], 1);
	ASSERT_EQ(arr[1], 2);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 1);
	ASSERT_EQ(arr[0], 2);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 0);
}

TEST(Int, RemoveFromMiddle)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	arr.Remove(1);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 2);
}

TEST(Int, Assign)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	arr[1] = 3;
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 3);
	ASSERT_EQ(arr[2], 2);
}

TEST(Int, ReadByIdxConst)
{
	Array<int> rawArr;
	rawArr.Insert(0);
	rawArr.Insert(1);
	rawArr.Insert(2);

	const Array<int> arr = rawArr;

	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 1);
	ASSERT_EQ(arr[2], 2);
}


TEST(Int, Iterator)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	std::vector<int> checkVec;

	for (auto it = arr.Iter(); it.HasNext(); it.Next())
		checkVec.push_back(it.Get());
	
	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], 0);
	ASSERT_EQ(checkVec[1], 1);
	ASSERT_EQ(checkVec[2], 2);
}

TEST(Int, IteratorReverse)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	std::vector<int> checkVec;

	for (auto it = arr.ReverseIter(); it.HasNext(); it.Next())
		checkVec.push_back(it.Get());

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], 2);
	ASSERT_EQ(checkVec[1], 1);
	ASSERT_EQ(checkVec[2], 0);
}

TEST(Int, IteratorConst)
{
	Array<int> rawArr;
	rawArr.Insert(0);
	rawArr.Insert(1);
	rawArr.Insert(2);

	const Array<int> arr = rawArr;
	std::vector<int> checkVec;

	for (auto it = arr.Iter(); it.HasNext(); it.Next())
		checkVec.push_back(it.Get());

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], 0);
	ASSERT_EQ(checkVec[1], 1);
	ASSERT_EQ(checkVec[2], 2);
}

TEST(Int, IteratorModify)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);
	
	for (auto it = arr.Iter(); it.HasNext(); it.Next())
		if (it.Get() == 1)
			it.Set(3);
	
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 3);
	ASSERT_EQ(arr[2], 2);
}

TEST(Int, IteratorInIterator)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	std::vector<int> checkVec;
	for (auto itA = arr.Iter(); itA.HasNext(); itA.Next())
		for (auto itB = arr.Iter(); itB.HasNext(); itB.Next())
			checkVec.push_back(itA.Get() * 10 + itB.Get());

	ASSERT_EQ(checkVec.size(), 9);
	ASSERT_EQ(checkVec[0], 0);
	ASSERT_EQ(checkVec[1], 1);
	ASSERT_EQ(checkVec[2], 2);
	ASSERT_EQ(checkVec[3], 10);
	ASSERT_EQ(checkVec[4], 11);
	ASSERT_EQ(checkVec[5], 12);
	ASSERT_EQ(checkVec[6], 20);
	ASSERT_EQ(checkVec[7], 21);
	ASSERT_EQ(checkVec[8], 22);
}


TEST(Int, IteratorForRangeModify)
{
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(1);
	arr.Insert(2);

	for (int& val : arr)
		val++;

	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], 1);
	ASSERT_EQ(arr[1], 2);
	ASSERT_EQ(arr[2], 3);
}

TEST(Int, IteratorConstForRange)
{
	Array<int> rawArr;
	rawArr.Insert(0);
	rawArr.Insert(1);
	rawArr.Insert(2);

	const Array<int> arr = rawArr;

	std::vector<int> checkVec;

	for (const int& val : arr)
		checkVec.push_back(val);

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], 0);
	ASSERT_EQ(checkVec[1], 1);
	ASSERT_EQ(checkVec[2], 2);
}


TEST(Int, CopyConstruct)
{
	Array<int> arrA;
	arrA.Insert(0);
	arrA.Insert(1);

	Array<int> arrB(arrA);

	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrA[0], arrB[0]);
	ASSERT_EQ(arrA[1], arrB[1]);
	ASSERT_EQ(arrA.GetCapacity(), arrB.GetCapacity());
	ASSERT_NE(arrA.GetItemsPointer(), arrB.GetItemsPointer());

	arrA.Remove(0);
	arrB.Insert(2);

	ASSERT_EQ(arrA.Size(), 1);
	ASSERT_EQ(arrA[0], 1);
	ASSERT_EQ(arrB.Size(), 3);
	ASSERT_EQ(arrB[0], 0);
	ASSERT_EQ(arrB[1], 1);
	ASSERT_EQ(arrB[2], 2);
}


TEST(Int, CopyAssign)
{
	Array<int> arrA;
	arrA.Insert(0);
	arrA.Insert(1);

	Array<int> arrB;
	arrB.Insert(3);
	arrB = arrA;

	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrA[0], arrB[0]);
	ASSERT_EQ(arrA[1], arrB[1]);
	ASSERT_EQ(arrA.GetCapacity(), arrB.GetCapacity());
	ASSERT_NE(arrA.GetItemsPointer(), arrB.GetItemsPointer());
}

TEST(Int, CopyAssignSelf)
{
	Array<int> arrA;
	arrA.Insert(0);
	arrA.Insert(1);
	const auto valuesPointer = arrA.GetItemsPointer();
	arrA = arrA;

	ASSERT_EQ(arrA.GetItemsPointer(), valuesPointer);
	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrA[0], 0);
	ASSERT_EQ(arrA[1], 1);
}

TEST(Int, MoveConstruct)
{
	Array<int> arrA;
	arrA.Insert(0);
	arrA.Insert(1);
	const int capA = arrA.GetCapacity();
	const int* itemsA = arrA.GetItemsPointer();

	Array<int> arrB(std::move(arrA));

	ASSERT_EQ(arrA.GetItemsPointer(), nullptr);
	ASSERT_EQ(arrB.GetItemsPointer(), itemsA);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB.GetCapacity(), capA);
	ASSERT_EQ(arrB[0], 0);
	ASSERT_EQ(arrB[1], 1);

	arrB.Remove(0);
	arrB.Insert(2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB[0], 1);
	ASSERT_EQ(arrB[1], 2);
}


TEST(Int, MoveAssign)
{
	Array<int> arrA;
	arrA.Insert(0);
	arrA.Insert(1);
	const int capA = arrA.GetCapacity();
	const int* itemsA = arrA.GetItemsPointer();

	Array<int> arrB;
	const int* itemsB = arrB.GetItemsPointer();
	arrB.Insert(2);
	arrB = std::move(arrA);

	ASSERT_EQ(arrA.GetItemsPointer(), itemsB);
	ASSERT_EQ(arrB.GetItemsPointer(), itemsA);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB.GetCapacity(), capA);
	ASSERT_EQ(arrB[0], 0);
	ASSERT_EQ(arrB[1], 1);
}

TEST(Int, MoveAssignSelf)
{
	Array<int> arrA;
	arrA.Insert(0);
	arrA.Insert(1);

	arrA = std::move(arrA);

	ASSERT_NE(arrA.GetItemsPointer(), nullptr);
	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrA[0], 0);
	ASSERT_EQ(arrA[1], 1);
}