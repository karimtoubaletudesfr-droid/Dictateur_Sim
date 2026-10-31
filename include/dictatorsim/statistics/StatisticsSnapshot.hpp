#pragma once
#include "dictatorsim/economy/Money.hpp"
#include <cstddef>
namespace ds {
    struct StatisticsSnapshot {
        std::uint64_t day=0;
        std::size_t population=0, activeProtesters=0, crimeIncidents=0;
        double averageHappiness=0, averageAnger=0, averageHealth=0, unemploymentRate=0;
        double economicProsperity=0, nationalStability=0, nationalWellbeing=0, militaryLoyalty=0, crimeRate=0;
        Money treasuryBalance=0, moneySupply=0, initialMoney=0;
        double totalProduction=0, priceIndex=1;
    };
    // Exact report today; later may be delayed/noisy without exposing World.
    struct GovernmentReport {
        StatisticsSnapshot observed;
    };
}
