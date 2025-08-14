#include "mylib/alg.hpp"
#include <immintrin.h>

namespace mylib {

std::uint64_t sum_of_squares(std::span<const std::uint32_t> xs) noexcept {
    // Portable scalar fallback; compilers will auto-vectorize for -O3.
    // (Kept simple to focus on template structure.)
    unsigned long long acc = 0ULL;
    for (std::uint32_t v : xs) {
        acc += static_cast<unsigned long long>(v) * static_cast<unsigned long long>(v);
    }
    return acc;
}

} // namespace mylib
