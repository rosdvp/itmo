#include "pch.h"

#include "DummyCopyAndMove.h"
#include "DummyCopyOnly.h"
#include "../dynamic_array/Array.h"

TEST(DummyCopyOnly, InsertAndRemove)
{
	Array<DummyCopyOnly> arr;
	arr.Insert(DummyCopyOnly(0));
	arr.Insert(DummyCopyOnly(1));

	ASSERT_EQ(arr[0].Val, 0);
	ASSERT_EQ(arr[1].Val, 1);

	arr.Remove(0);
	ASSERT_EQ(arr[0].Val, 1);
}

TEST(DummyCopyOnly, RemoveAndCheckDestruction)
{
	Array<DummyCopyOnly> arr;
	arr.Insert(DummyCopyOnly(0));
	arr.Insert(DummyCopyOnly(1));

	const DummyCopyOnly* pLast = &arr[1];

	arr.Remove(1);

	ASSERT_EQ(pLast->IsDestructed, true);
}

TEST(DummyCopyOnly, InsertAndCheckReplace)
{
	Array<DummyCopyOnly> arr;
	arr.Insert(DummyCopyOnly(1));

	const DummyCopyOnly* pLast = &arr[0];

	arr.Insert(0, DummyCopyOnly(2));

	ASSERT_EQ(pLast->Val, 2);
	ASSERT_EQ(arr[1].IsAssignUsed, true);
}


TEST(DummyCopyAndMove, InsertAndRemove)
{
	Array<DummyCopyAndMove> arr;
	arr.Insert(DummyCopyAndMove(0));
	arr.Insert(DummyCopyAndMove(1));

	ASSERT_EQ(arr[0].Val, 0);
	ASSERT_EQ(arr[1].Val, 1);

	arr.Remove(0);
	ASSERT_EQ(arr[0].Val, 1);
}

TEST(DummyCopyAndMove, InsertAndCheckReplace)
{
	Array<DummyCopyAndMove> arr;
	arr.Insert(DummyCopyAndMove(1));

	const DummyCopyAndMove* pLast = &arr[0];

	arr.Insert(0, DummyCopyAndMove(2));

	ASSERT_EQ(pLast->Val, 2);
	ASSERT_EQ(arr[1].CreationType, DummyCopyAndMove::ECreationType::MoveAssign);
}