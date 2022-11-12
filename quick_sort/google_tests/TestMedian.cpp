#include <tuple>
#include <gtest/gtest.h>
#include "../src/QuickSort.h"
#include "../src/TestHelpers.h"

using MedianValues = std::tuple<int, int, int, int>;

class MedianFixture : public ::testing::TestWithParam<MedianValues>
{
};

INSTANTIATE_TEST_CASE_P(Median, MedianFixture, ::testing::Values(
	MedianValues(1, 1, 1, 1),
	MedianValues(1, 1, 2, 1),
	MedianValues(1, 2, 1, 1),
	MedianValues(2, 1, 1, 1),
	MedianValues(1, 2, 3, 2),
	MedianValues(1, 3, 2, 2),
	MedianValues(2, 1, 3, 2),
	MedianValues(3, 1, 2, 2),
	MedianValues(2, 3, 1, 2),
	MedianValues(3, 2, 1, 2)
	));


TEST_P(MedianFixture, Median)
{
	auto& [v0, v1, v2, r] = GetParam();
	const int* result = GetMedian(&v0, &v1, &v2, [](const int& a, const int& b) { return a < b; });
	ASSERT_EQ(*result, r);
}