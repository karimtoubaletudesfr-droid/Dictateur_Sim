#include "dictatorsim/decisions/HumanDecisionProvider.hpp"
namespace ds {
    void HumanDecisionProvider::request(const DecisionRequest&) {
        /* The UI receives reports through Simulation::report(). */
    }
    void HumanDecisionProvider::submit(DecisionResult r) {
        pending_.push_back(r);
    }
    std::optional<DecisionResult> HumanDecisionProvider::poll() {
        if(pending_.empty())return {
        };
        auto r=pending_.front();
        pending_.pop_front();
        return r;
    }
}
