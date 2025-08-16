#include "mylib/alg.hpp"

#include <benchmark/benchmark.h>
#include <random>
#include <vector>

static void BM_SumOfSquares(benchmark::State& state)
{
    std::size_t N = static_cast<std::size_t>(state.range(0));
    std::vector<std::uint32_t> data(N);
    std::mt19937 rng(42);
    std::uniform_int_distribution<std::uint32_t> dist(0, 1000);
    for (auto& v : data)
        v = dist(rng);

    for (auto _ : state) {
        auto res = mylib::sum_of_squares(data);
        benchmark::DoNotOptimize(res);
    }

    state.SetItemsProcessed(static_cast<long long>(state.iterations()) * static_cast<long long>(N));
}

BENCHMARK(BM_SumOfSquares)->Arg(1 << 10)->Arg(1 << 16)->Arg(1 << 20);
BENCHMARK_MAIN();
