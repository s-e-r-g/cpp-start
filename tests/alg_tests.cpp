#include "mylib/alg.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(Alg, SmallVectors)
{
    std::vector<std::uint32_t> v {0, 1, 2, 3};
    EXPECT_EQ(mylib::sum_of_squares(v), 0ULL + 1ULL + 4ULL + 9ULL);
}

TEST(Alg, LargeValues)
{
    std::vector<std::uint32_t> v {4000000000u, 2u};
    // 4e9^2 = 16e18 (fits into 64-bit); check no overflow into 32-bit.
    unsigned long long expected = 16000000000000000000ULL + 4ULL;
    EXPECT_EQ(mylib::sum_of_squares(v), expected);
}
