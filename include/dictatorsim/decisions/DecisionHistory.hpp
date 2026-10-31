#pragma once
#include "dictatorsim/statistics/StatisticsSnapshot.hpp"
#include "dictatorsim/politics/PoliticalAction.hpp"
#include <vector>
#include <optional>
#include <string>
namespace ds {
struct DecisionRecord {
    std::uint64_t day=0;
    PoliticalAction action;
    bool accepted=false;
    Money immediateCost=0;
    std::string explanation;
    StatisticsSnapshot before, after;
    std::optional<StatisticsSnapshot> afterThirtyDays;
};
class DecisionHistory {
    std::vector<DecisionRecord> records_;
public:
    void add(DecisionRecord record);
    void observe(const StatisticsSnapshot& snapshot);
    const std::vector<DecisionRecord>& records() const;
};
}
