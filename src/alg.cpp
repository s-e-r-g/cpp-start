#include "mylib/alg.hpp"

namespace mylib {

std::uint64_t sum_of_squares(std::span<const std::uint32_t> xs) noexcept
{
    std::uint64_t acc = 0;
    for (const auto v : xs) {
        acc += static_cast<std::uint64_t>(v) * static_cast<std::uint64_t>(v);
    }
    return acc;
}

} // namespace mylib
