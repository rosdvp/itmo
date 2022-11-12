#include <benchmark/benchmark.h>
#include <chrono>
#include "../src/QuickSort.h"
#include "../src/TestHelpers.h"

static void BM_InsertSort(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vect(state.range(0), 0);
        benchmark::DoNotOptimize(vect.data());
        benchmark::ClobberMemory();
        FillIntArrayBackwards(vect.data(), vect.size());

        auto start = std::chrono::high_resolution_clock::now();

        InsertSort(vect.begin(), vect.end(), [](const int& a, const int& b) { return a < b; });

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds =
                std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
}
BENCHMARK(BM_InsertSort)->DenseRange(2, 16, 2)->UseManualTime();

static void BM_QuickSort(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vect(state.range(0), 0);
        benchmark::DoNotOptimize(vect.data());
        benchmark::ClobberMemory();
        FillIntArrayBackwards(vect.data(), vect.size());

        auto start = std::chrono::high_resolution_clock::now();

        Sort(vect.begin(), vect.end(), [](const int& a, const int& b) { return a < b; });

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds =
                std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
}
BENCHMARK(BM_QuickSort)->DenseRange(2, 16, 2)->UseManualTime();

BENCHMARK_MAIN();