#include "pch.h"
#include "gtest/gtest-typed-test.h"
#include "../dynamic_array/Array.h"
#include "DummyWithCopyOnly.h"
#include "DummyWithCopyAndMove.h"

template <typename T>
class ArrayTest : public ::testing::Test
{
public:
	T* Vals;
protected:
	void SetUp() override
	{
		Vals = new T[10];
		for (int i = 0; i < 10; i++)
		{
			T t(i);
			Vals[i] = t;
		}
	}
	void TearDown() override
	{
		delete[] Vals;
		Vals = nullptr;
	}
};

using TestTypes = ::testing::Types<int, DummyWithCopyOnly, DummyWithCopyAndMove>;
TYPED_TEST_CASE(ArrayTest, TestTypes, );

TYPED_TEST(ArrayTest, CreateEmpty)
{
	Array<TypeParam> arr;
	ASSERT_EQ(arr.Size(), 0);
}

TYPED_TEST(ArrayTest, InsertAtEnd)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	ASSERT_EQ(arr.Size(), 1);
	arr.Insert(this->Vals[1]);
	ASSERT_EQ(arr.Size(), 2);
	arr.Insert(this->Vals[2]);
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[1]);
	ASSERT_EQ(arr[2], this->Vals[2]);
}

/*
TYPED_TEST(ArrayTest, InsertAtBegin)
{
	Array<TypeParam> arr;
	arr.Insert(0, this->Vals[2]);
	ASSERT_EQ(arr.Size(), 1);
	arr.Insert(0, this->Vals[1]);
	ASSERT_EQ(arr.Size(), 2);
	arr.Insert(0, this->Vals[0]);
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], 0);
	ASSERT_EQ(arr[1], 1);
	ASSERT_EQ(arr[2], 2);
}

TYPED_TEST(ArrayTest, InsertAtMiddle)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[3]);
	arr.Insert(this->Vals[4]);
	arr.Insert(2, this->Vals[2]);
	ASSERT_EQ(arr.Size(), 5);
	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[1]);
	ASSERT_EQ(arr[2], this->Vals[2]);
	ASSERT_EQ(arr[3], this->Vals[3]);
	ASSERT_EQ(arr[4], this->Vals[4]);
}

TYPED_TEST(ArrayTest, RemoveAtEnd)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	arr.Remove(2);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[1]);

	arr.Remove(1);
	ASSERT_EQ(arr.Size(), 1);
	ASSERT_EQ(arr[0], this->Vals[0]);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 0);
}

TYPED_TEST(ArrayTest, RemoveAtBegin)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], this->Vals[1]);
	ASSERT_EQ(arr[1], this->Vals[2]);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 1);
	ASSERT_EQ(arr[0], this->Vals[2]);

	arr.Remove(0);
	ASSERT_EQ(arr.Size(), 0);
}

TYPED_TEST(ArrayTest, RemoveAtMiddle)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	arr.Remove(1);
	ASSERT_EQ(arr.Size(), 2);
	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[2]);
}

TYPED_TEST(ArrayTest, Assign)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	arr[1] = this->Vals[3];
	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[3]);
	ASSERT_EQ(arr[2], this->Vals[2]);
}

TYPED_TEST(ArrayTest, ReadByIdxConst)
{
	Array<TypeParam> rawArr;
	rawArr.Insert(this->Vals[0]);
	rawArr.Insert(this->Vals[1]);
	rawArr.Insert(this->Vals[2]);

	const Array<TypeParam> arr = rawArr;

	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[1]);
	ASSERT_EQ(arr[2], this->Vals[2]);
}


TYPED_TEST(ArrayTest, Iterator)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	std::vector<TypeParam> checkVec;

	for (auto it = arr.Iter(); it.HasNext(); it.Next())
		checkVec.push_back(it.Get());

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], this->Vals[0]);
	ASSERT_EQ(checkVec[1], this->Vals[1]);
	ASSERT_EQ(checkVec[2], this->Vals[2]);
}

TYPED_TEST(ArrayTest, IteratorReverse)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	std::vector<TypeParam> checkVec;

	for (auto it = arr.ReverseIter(); it.HasNext(); it.Next())
		checkVec.push_back(it.Get());

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], this->Vals[2]);
	ASSERT_EQ(checkVec[1], this->Vals[1]);
	ASSERT_EQ(checkVec[2], this->Vals[0]);
}

TYPED_TEST(ArrayTest, IteratorConst)
{
	Array<TypeParam> rawArr;
	rawArr.Insert(this->Vals[0]);
	rawArr.Insert(this->Vals[1]);
	rawArr.Insert(this->Vals[2]);

	const Array<TypeParam> arr = rawArr;
	std::vector<TypeParam> checkVec;

	for (auto it = arr.Iter(); it.HasNext(); it.Next())
		checkVec.push_back(it.Get());

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], this->Vals[0]);
	ASSERT_EQ(checkVec[1], this->Vals[1]);
	ASSERT_EQ(checkVec[2], this->Vals[2]);
}

TYPED_TEST(ArrayTest, IteratorModify)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	for (auto it = arr.Iter(); it.HasNext(); it.Next())
		if (it.Get() == this->Vals[1])
			it.Set(this->Vals[3]);

	ASSERT_EQ(arr[0], this->Vals[0]);
	ASSERT_EQ(arr[1], this->Vals[3]);
	ASSERT_EQ(arr[2], this->Vals[2]);
}

TYPED_TEST(ArrayTest, IteratorInIterator)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	std::vector<TypeParam> checkVec;
	for (auto itA = arr.Iter(); itA.HasNext(); itA.Next())
	{
		checkVec.push_back(itA.Get());
		for (auto itB = arr.Iter(); itB.HasNext(); itB.Next())
			checkVec.push_back(itB.Get());
	}

	ASSERT_EQ(checkVec.size(), 12);
	ASSERT_EQ(checkVec[0], this->Vals[0]);
	ASSERT_EQ(checkVec[1], this->Vals[0]);
	ASSERT_EQ(checkVec[2], this->Vals[1]);
	ASSERT_EQ(checkVec[3], this->Vals[2]);
	ASSERT_EQ(checkVec[4], this->Vals[1]);
	ASSERT_EQ(checkVec[5], this->Vals[0]);
	ASSERT_EQ(checkVec[6], this->Vals[1]);
	ASSERT_EQ(checkVec[7], this->Vals[2]);
	ASSERT_EQ(checkVec[8], this->Vals[2]);
	ASSERT_EQ(checkVec[9], this->Vals[0]);
	ASSERT_EQ(checkVec[10], this->Vals[1]);
	ASSERT_EQ(checkVec[11], this->Vals[2]);
}


TYPED_TEST(ArrayTest, IteratorForRangeModify)
{
	Array<TypeParam> arr;
	arr.Insert(this->Vals[0]);
	arr.Insert(this->Vals[1]);
	arr.Insert(this->Vals[2]);

	int i = 3;
	for (TypeParam& val : arr)
		val = this->Vals[i++];

	ASSERT_EQ(arr.Size(), 3);
	ASSERT_EQ(arr[0], this->Vals[3]);
	ASSERT_EQ(arr[1], this->Vals[4]);
	ASSERT_EQ(arr[2], this->Vals[5]);
}

TYPED_TEST(ArrayTest, IteratorConstForRange)
{
	Array<TypeParam> rawArr;
	rawArr.Insert(this->Vals[0]);
	rawArr.Insert(this->Vals[1]);
	rawArr.Insert(this->Vals[2]);

	const Array<TypeParam> arr = rawArr;

	std::vector<TypeParam> checkVec;

	for (const TypeParam& val : arr)
		checkVec.push_back(val);

	ASSERT_EQ(checkVec.size(), 3);
	ASSERT_EQ(checkVec[0], this->Vals[0]);
	ASSERT_EQ(checkVec[1], this->Vals[1]);
	ASSERT_EQ(checkVec[2], this->Vals[2]);
}


TYPED_TEST(ArrayTest, CopyConstruct)
{
	Array<TypeParam> arrA;
	arrA.Insert(this->Vals[0]);
	arrA.Insert(this->Vals[1]);

	Array<TypeParam> arrB(arrA);

	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrA[0], arrB[0]);
	ASSERT_EQ(arrA[1], arrB[1]);
	ASSERT_EQ(arrA.GetCapacity(), arrB.GetCapacity());
	ASSERT_NE(arrA.GetValuesPointer(), arrB.GetValuesPointer());

	arrA.Remove(0);
	arrB.Insert(this->Vals[2]);

	ASSERT_EQ(arrA.Size(), 1);
	ASSERT_EQ(arrA[0], this->Vals[1]);
	ASSERT_EQ(arrB.Size(), 3);
	ASSERT_EQ(arrB[0], this->Vals[0]);
	ASSERT_EQ(arrB[1], this->Vals[1]);
	ASSERT_EQ(arrB[2], this->Vals[2]);
}


TYPED_TEST(ArrayTest, CopyAssign)
{
	Array<TypeParam> arrA;
	arrA.Insert(this->Vals[0]);
	arrA.Insert(this->Vals[1]);

	Array<TypeParam> arrB;
	arrB.Insert(this->Vals[2]);
	arrB = arrA;

	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrA[0], arrB[0]);
	ASSERT_EQ(arrA[1], arrB[1]);
	ASSERT_EQ(arrA.GetCapacity(), arrB.GetCapacity());
	ASSERT_NE(arrA.GetValuesPointer(), arrB.GetValuesPointer());
}

TYPED_TEST(ArrayTest, CopyAssignSelf)
{
	Array<TypeParam> arrA;
	arrA.Insert(this->Vals[0]);
	arrA.Insert(this->Vals[1]);
	const auto valuesPointer = arrA.GetValuesPointer();
	arrA = arrA;

	ASSERT_EQ(arrA.GetValuesPointer(), valuesPointer);
	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrA[0], this->Vals[0]);
	ASSERT_EQ(arrA[1], this->Vals[1]);
}

TYPED_TEST(ArrayTest, MoveConstruct)
{
	Array<TypeParam> arrA;
	arrA.Insert(this->Vals[0]);
	arrA.Insert(this->Vals[1]);
	const int capA = arrA.GetCapacity();

	Array<TypeParam> arrB(std::move(arrA));

	ASSERT_EQ(arrA.GetValuesPointer(), nullptr);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB.GetCapacity(), capA);
	ASSERT_EQ(arrB[0], this->Vals[0]);
	ASSERT_EQ(arrB[1], this->Vals[1]);

	arrB.Remove(0);
	arrB.Insert(this->Vals[2]);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB[0], this->Vals[1]);
	ASSERT_EQ(arrB[1], this->Vals[2]);
}


TYPED_TEST(ArrayTest, MoveAssign)
{
	Array<TypeParam> arrA;
	arrA.Insert(this->Vals[0]);
	arrA.Insert(this->Vals[1]);
	const int capA = arrA.GetCapacity();

	Array<TypeParam> arrB;
	arrB.Insert(this->Vals[2]);
	arrB = std::move(arrA);

	ASSERT_EQ(arrA.GetValuesPointer(), nullptr);
	ASSERT_EQ(arrB.Size(), 2);
	ASSERT_EQ(arrB.GetCapacity(), capA);
	ASSERT_EQ(arrB[0], this->Vals[0]);
	ASSERT_EQ(arrB[1], this->Vals[1]);
}

TYPED_TEST(ArrayTest, MoveAssignSelf)
{
	Array<TypeParam> arrA;
	arrA.Insert(this->Vals[0]);
	arrA.Insert(this->Vals[1]);

	arrA = std::move(arrA);

	ASSERT_NE(arrA.GetValuesPointer(), nullptr);
	ASSERT_EQ(arrA.Size(), 2);
	ASSERT_EQ(arrA[0], this->Vals[0]);
	ASSERT_EQ(arrA[1], this->Vals[1]);
}
*/