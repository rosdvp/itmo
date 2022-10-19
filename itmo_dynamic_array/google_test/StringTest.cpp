#include "pch.h"
#include "../dynamic_array/Array.h"


TEST(String, CreateEmpty)
{
	Array<std::string> arr;
	ASSERT_EQ(arr.Size(), 0);
}

TEST(String, InsertAtEnd)
{
	Array<std::string> arr;
	arr.Insert("0");
	ASSERT_EQ(arr.Size(), 1);
	arr.Insert("11");
	ASSERT_EQ(arr.Size(), 2);
	arr.Insert("222");
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], "0");
	ASSERT_EQ(arr[1], "11");
	ASSERT_EQ(arr[2], "222");
}

TEST(String, InsertAtBegin)
{
	Array<std::string> arr;
	arr.Insert(0, "222");
	ASSERT_EQ(arr.Size(), 1);
	arr.Insert(0, "11");
	ASSERT_EQ(arr.Size(), 2);
	arr.Insert(0, "0");
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], "0");
	ASSERT_EQ(arr[1], "11");
	ASSERT_EQ(arr[2], "222");
}

TEST(String, InsertAtMiddle)
{
	Array<std::string> arr;
	arr.Insert("0");
	arr.Insert("11");
	arr.Insert("3333");
	arr.Insert("44444");
	arr.Insert(2, "222");
	ASSERT_EQ(arr.Size(), 5);
	ASSERT_EQ(arr[0], "0");
	ASSERT_EQ(arr[1], "11");
	ASSERT_EQ(arr[2], "222");
	ASSERT_EQ(arr[3], "3333");
	ASSERT_EQ(arr[4], "44444");
}

TEST(String, IncreaseCapacity)
{
	Array<std::string> arr;

	const int initCapacity = arr.GetCapacity();
	for (int i = 0; i < 100; i++)
		arr.Insert(std::to_string(i));

	ASSERT_LT(initCapacity, arr.GetCapacity());
	ASSERT_EQ(arr.Size(), 100);
	for (int i = 0; i < 100; i++)
		ASSERT_EQ(arr[i], std::to_string(i));
}

TEST(String, RemoveFromEnd)
{
	Array<std::string> arr;
	arr.Insert("0");
	arr.Insert("11");
	arr.Insert("222");

	arr.Remove(2);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], "0");
	ASSERT_EQ(arr[1], "11");

	arr.Remove(1);
	ASSERT_EQ(arr.Size(), 1);
	ASSERT_EQ(arr[0], "0");

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 0);
}

TEST(String, RemoveFromBegin)
{
	Array<std::string> arr;
	arr.Insert("0");
	arr.Insert("11");
	arr.Insert("222");

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], "11");
	ASSERT_EQ(arr[1], "222");

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 1);
	ASSERT_EQ(arr[0], "222");

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 0);
}

TEST(String, RemoveFromMiddle)
{
	Array<std::string> arr;
	arr.Insert("0");
	arr.Insert("11");
	arr.Insert("222");

	arr.Remove(1);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], "0");
	ASSERT_EQ(arr[1], "222");
}

TEST(String, Assign)
{
	Array<std::string> arr;
	arr.Insert("0");
	arr.Insert("11");
	arr.Insert("222");

	arr[1] = "3333";
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], "0");
	ASSERT_EQ(arr[1], "3333");
	ASSERT_EQ(arr[2], "222");
}


TEST(String, CopyConstruct)
{
	Array<std::string> arrA;
	arrA.Insert("0");
	arrA.Insert("11");

	Array<std::string> arrB(arrA);

	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrA[0], arrB[0]);
	ASSERT_EQ(arrA[1], arrB[1]);
	ASSERT_EQ(arrA.GetCapacity(), arrB.GetCapacity());
	ASSERT_NE(arrA.GetValuesPointer(), arrB.GetValuesPointer());

	arrA.Remove(0);
	arrB.Insert("222");

	ASSERT_EQ(arrA.Size(), 1);
	ASSERT_EQ(arrA[0], "11");
	ASSERT_EQ(arrB.Size(), 3);
	ASSERT_EQ(arrB[0], "0");
	ASSERT_EQ(arrB[1], "11");
	ASSERT_EQ(arrB[2], "222");
}


TEST(String, CopyAssign)
{
	Array<std::string> arrA;
	arrA.Insert("0");
	arrA.Insert("11");

	Array<std::string> arrB;
	arrB.Insert("3333");
	arrB = arrA;

	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrA[0], arrB[0]);
	ASSERT_EQ(arrA[1], arrB[1]);
	ASSERT_EQ(arrA.GetCapacity(), arrB.GetCapacity());
	ASSERT_NE(arrA.GetValuesPointer(), arrB.GetValuesPointer());
}

TEST(String, CopyAssignSelf)
{
	Array<std::string> arrA;
	arrA.Insert("0");
	arrA.Insert("11");
	const auto valuesPointer = arrA.GetValuesPointer();
	arrA = arrA;

	ASSERT_EQ(arrA.GetValuesPointer(), valuesPointer);
	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrA[0], "0");
	ASSERT_EQ(arrA[1], "11");
}

TEST(String, MoveConstruct)
{
	Array<std::string> arrA;
	arrA.Insert("0");
	arrA.Insert("11");
	const int capA = arrA.GetCapacity();

	Array<std::string> arrB(std::move(arrA));

	ASSERT_EQ(arrA.GetValuesPointer(), nullptr);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB.GetCapacity(), capA);
	ASSERT_EQ(arrB[0], "0");
	ASSERT_EQ(arrB[1], "11");

	arrB.Remove(0);
	arrB.Insert("222");
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB[0], "11");
	ASSERT_EQ(arrB[1], "222");
}


TEST(String, MoveAssign)
{
	Array<std::string> arrA;
	arrA.Insert("0");
	arrA.Insert("11");
	const int capA = arrA.GetCapacity();

	Array<std::string> arrB;
	arrB.Insert("222");
	arrB = std::move(arrA);

	ASSERT_EQ(arrA.GetValuesPointer(), nullptr);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB.GetCapacity(), capA);
	ASSERT_EQ(arrB[0], "0");
	ASSERT_EQ(arrB[1], "11");
}

TEST(String, MoveAssignSelf)
{
	Array<std::string> arrA;
	arrA.Insert("0");
	arrA.Insert("11");

	arrA = std::move(arrA);

	ASSERT_NE(arrA.GetValuesPointer(), nullptr);
	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrA[0], "0");
	ASSERT_EQ(arrA[1], "11");
}