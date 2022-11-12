#pragma once
#include <functional>
#include <iostream>

constexpr int COUNT_FOR_INSERT_SORT = 8;

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

    for (T cursor = first + 1; cursor <= last; ++cursor)
        if (comp(*cursor, *(cursor - 1)))
        {
            T insertCursor = cursor;
            do
            {
                std::swap(*insertCursor, *(insertCursor-1));
                --insertCursor;
            } while (insertCursor != first && comp(*insertCursor, *(insertCursor - 1)));
        }
}


/**
 * \brief For internal use only!
 */
template<typename T, typename Compare>
T QuickSortPartition(T first, T last, const int count, Compare comp)
{
    //micro-optimization for case of two values
    if (count == 2)
    {
        if (!comp(*first, *last))
            std::swap(*first, *last);
        return first;
    }

    T middle = first + count / 2;
    T pivot = GetMedian(first, middle, last, comp);

    //put pivot at the end to exclude it from swapping (will be returned back later)
    if (pivot != last)
    {
        std::swap(*pivot, *last);
        pivot = last;
    }
    --last;

    //when first and last meet each other,
    //any left-side element will be < pivot and any right-side element >= pivot
    while (first != last)
    {
        if (comp(*first, *pivot)) //first is already smaller, skip it
            first += 1;
        else if (comp(*pivot, *last)) //last is already greater, skip it
            last -= 1;
        else
        {
            std::swap(*first, *last);
            first += 1; //to avoid dead loop, and to be sure that meet-point >= pivot
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
        if (count <= 1)
            return;
        if (count <= COUNT_FOR_INSERT_SORT)
        {
            InsertSortImpl(first, last, comp);
            return;
        }

        T pivot = QuickSortPartition(first, last, count, comp);

        const int countLeft = pivot - first;
        const int countRight = last - pivot;

        //choose smallest part and run recursion on it
        if (countLeft < countRight)
        {
            if (countLeft > 0) //only if left partition exists
                QuickSortRec(first, pivot - 1, comp);
            count = countRight;
            first = pivot + 1;
        }
        else
        {
            if (countRight > 0) //only if right partition exists
                QuickSortRec(pivot + 1, last, comp);
            count = countLeft;
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