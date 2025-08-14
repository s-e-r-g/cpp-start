#include "mylib/alg.hpp"
#include "perf_counters.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#ifdef HAVE_LINUX_PERF
  #include <linux/perf_event.h>
#endif

int main(){
    // Prepare some data
    const std::size_t N = 1'000'000;
    std::vector<std::uint32_t> data(N);
    std::mt19937 rng(123);
    std::uniform_int_distribution<std::uint32_t> dist(0, 1000);
    for(auto& v: data) v = dist(rng);

    PerfSession perf;
#ifdef HAVE_LINUX_PERF
    std::vector<PerfCounterSpec> specs = {
        {"cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES},
        {"instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS},
        {"cache-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES}
    };
#else
    std::vector<PerfCounterSpec> specs = {};
#endif
    if(!perf.open(specs)){
        std::cerr << "Perf counters unavailable; continuing without.\n";
    }

    auto t0 = std::chrono::high_resolution_clock::now();
    perf.start();
    auto result = mylib::sum_of_squares(data);
    perf.stop();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "sum_of_squares = " << result << "\n";
    std::cout << "wall time: "
              << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";

    for(const auto& r: perf.read()){
        std::cout << r.name << ": " << r.value << "\n";
    }

    return 0;
}
