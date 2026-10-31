#pragma once
#include "dictatorsim/statistics/StatisticsSnapshot.hpp"
#include "DecisionOption.hpp"
#include <vector>
namespace ds {
    struct DecisionRequest {
        std::uint64_t id;
        GovernmentReport report;
        std::string prompt="";
        std::vector<DecisionOption> options{};
    };
}
