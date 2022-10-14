#pragma once
#include <functional>
#include <iostream>

constexpr int COUNT_FOR_INSERT_SORT = 5;


template<typename T, typename Compare>
T GetMedian(const T& a, const T& b, const T& c, Compare comp)
{
	if (comp(*a, *b))
	{
		if (comp(*b, *c))
			return b;
		return comp(*a, *c) ? c : a;
	}
	if (comp(*a, *c))
		return a;
	return comp(*b, *c) ? c : b;
}

/**
 * \brief For internal use only!
 */
template<typename T, typename Compare>
void InsertSortImpl(const T first, const T last, Compare comp)
{
	if (first == last) //only 1 item
		return;

	T cursor = first;
	do
	{
		cursor += 1;
		if (comp(*cursor, *(cursor - 1)))
		{
			T insertCursor = cursor;
			do
			{
				std::swap(*insertCursor, *(insertCursor - 1));
				insertCursor -= 1;
			} while (insertCursor != first && comp(*insertCursor, *(insertCursor - 1)));
		}
	} while (cursor != last);
}


/**
 * \brief For internal use only!
 */
template<typename T, typename Compare>
T QuickSortPartition(T first, T last, const int count, Compare comp)
{
	T middle = first + count / 2;
	T pivot = GetMedian(first, middle, last, comp);

	if (pivot != last)
	{
		std::swap(*pivot, *last);
		pivot = last;
	}
	--last; //exclude pivot from swapping

	while (first != last)
	{
		if (comp(*first, *pivot))
			first += 1;
		else if (comp(*pivot, *last))
			last -= 1;
		else
		{
			std::swap(*first, *last);
			first += 1; //to avoid dead loop
		}
	}
	std::swap(*first, *pivot);

	return first;
}

/**
 * \brief For internal use only!
 */
template<typename T, typename Compare>
void QuickSortRec(T first, T last, Compare comp)
{
	int count = last - first + 1;
	while (true)
	{
		if (count <= COUNT_FOR_INSERT_SORT)
		{
			InsertSortImpl(first, last, comp);
			return;
		}

		T pivot = QuickSortPartition(first, last, count, comp);

		const int countLeft = pivot - first;
		const int countRight = last - pivot;

		if (countLeft < countRight)
		{
			if (countLeft > 0) //only if left partition exists
				QuickSortRec(first, pivot - 1, comp);
			count = countLeft;
			first = pivot + 1;
		}
		else
		{
			if (countRight > 0) //only if right partition exists
				QuickSortRec(pivot + 1, last, comp);
			count = countRight;
			last = pivot - 1;
		}
	}
}

/**
 * \brief sort range [first, last)
 * \tparam T Item type
 * \tparam Compare Predicate must be (const T& A, const T& B)->bool
 * \param first Pointer on the first item in range (or vector.begin())
 * \param last Pointer on the item AFTER last in range (or vector.end())
 * \param comp Return true if A is less than B
 */
template<typename T, typename Compare>
void InsertSort(T first, T last, Compare comp)
{
	if (first == last) //empty collection
		return;
	InsertSortImpl(first, last-1, comp);
}

/**
 * \brief sort range [first, last)
 * \tparam T Item type
 * \tparam Compare Predicate must be (const T& A, const T& B)->bool
 * \param first Pointer on the first item in range (or vector.begin())
 * \param last Pointer on the item AFTER last in range (or vector.end())
 * \param comp Return true if A is less than B
 */
template<typename T, typename Compare>
void Sort(T first, T last, Compare comp)
{
	if (first == last) //empty collection
		return;
	--last; //because I prefer last actually be last item in collection
	if (first == last) //only 1 item
		return;
	QuickSortRec(first, last, comp);
}