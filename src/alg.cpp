#include "mylib/alg.hpp"

#include <immintrin.h>

namespace mylib {

std::uint64_t sum_of_squares(std::span<const std::uint32_t> xs) noexcept
{
    // Portable scalar fallback; compilers will auto-vectorize for -O3.
    // (Kept simple to focus on template structure.)
    std::uint64_t acc = 0;
    for (const auto v : xs) {
        acc += static_cast<std::uint64_t>(v) * static_cast<std::uint64_t>(v);
    }
    return acc;
}

} // namespace mylib
