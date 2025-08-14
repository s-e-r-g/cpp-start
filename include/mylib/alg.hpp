#pragma once
#include <span>
#include <cstdint>

namespace mylib {

// Example: sum of squares with simple branchless reduction.
// Intentionally written to be testable and benchmarkable.
std::uint64_t sum_of_squares(std::span<const std::uint32_t> xs) noexcept;

}
