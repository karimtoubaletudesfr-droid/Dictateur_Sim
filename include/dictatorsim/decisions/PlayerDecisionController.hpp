#pragma once
#include "DecisionHistory.hpp"
#include "DecisionResult.hpp"
#include "DecisionOption.hpp"
namespace ds {
class Simulation;
class PlayerDecisionController {
    DecisionHistory history_;
public:
    DecisionResult submit(Simulation& simulation,const PoliticalAction& action);
    void observe(const StatisticsSnapshot& snapshot);
    const DecisionHistory& history() const;
    static const std::vector<DecisionOption>& catalog();
};
}
