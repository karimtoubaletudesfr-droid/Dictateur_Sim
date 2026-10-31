#include "dictatorsim/core/Simulation.hpp"
#include "dictatorsim/decisions/HumanDecisionProvider.hpp"
#include "dictatorsim/decisions/HybridDecisionProvider.hpp"
#include "TestWorld.hpp"
int main() {
    ds::HumanDecisionProvider human;
    ds::HybridDecisionProvider hybrid(human);
    human.submit({0,{ds::ActionType::IncreaseTaxes,0,100},true});
    assert(!hybrid.poll());
    assert(hybrid.proposal());
    hybrid.approve(false);
    assert(!hybrid.poll());
    human.submit({1,{ds::ActionType::IncreaseTaxes,0,100},true});
    assert(!hybrid.poll());
    hybrid.approve(true);
    auto result=hybrid.poll();
    assert(result && result->action.basisPoints==100);
    assert(!hybrid.poll());
}
