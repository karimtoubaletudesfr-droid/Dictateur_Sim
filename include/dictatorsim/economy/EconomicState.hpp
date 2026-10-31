#pragma once
#include "Money.hpp"
#include <cstddef>
namespace ds {
    struct EconomicState {
        double totalProduction=0, priceIndex=1, inflation=0, prosperity=0;
        std::size_t purchases=0, refused=0;
        Money wagesPaid=0;
        double agriculturalMultiplier=1;
        std::uint64_t droughtUntil=0;
    };
}
