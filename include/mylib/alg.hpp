#pragma once
#include <cstdint>
#include <span>

namespace mylib {

// Example: sum of squares with simple branchless reduction.
// Intentionally written to be testable and benchmarkable.
std::uint64_t sum_of_squares(std::span<const std::uint32_t> xs) noexcept;

}
