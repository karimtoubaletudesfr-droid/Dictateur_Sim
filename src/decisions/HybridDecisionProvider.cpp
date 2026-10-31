#include "dictatorsim/decisions/HybridDecisionProvider.hpp"
namespace ds {
    HybridDecisionProvider::HybridDecisionProvider(IDecisionProvider& p):proposals_(p) {
    }
    void HybridDecisionProvider::request(const DecisionRequest& r) {
        proposals_.request(r);
    }
    std::optional<DecisionResult> HybridDecisionProvider::poll() {
        if(!awaiting_)awaiting_=proposals_.poll();
        if(!approved_)return {
        };
        auto result=awaiting_;
        awaiting_.reset();
        approved_=false;
        return result;
    }
    std::optional<DecisionResult> HybridDecisionProvider::proposal()const {
        return awaiting_;
    }
    void HybridDecisionProvider::approve(bool yes) {
        if(!awaiting_)return;
        if(yes)approved_=true;
        else {
            awaiting_.reset();
            approved_=false;
        }
    }
}
