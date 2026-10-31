#pragma once
#include <cstdint>
namespace ds {
    using Money = std::int64_t;
    using AccountId = std::uint32_t;
    // Basis points, rounded down; avoids intermediate multiplication overflow.
    Money fraction(Money amount, int basisPoints);
}
