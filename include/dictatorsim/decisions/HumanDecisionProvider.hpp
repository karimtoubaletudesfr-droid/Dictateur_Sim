#pragma once
#include "IDecisionProvider.hpp"
#include <deque>
namespace ds {
    // Single-thread event-loop queue. A graphical controller submits on its owning thread.
    class HumanDecisionProvider: public IDecisionProvider {
        std::deque<DecisionResult> pending_;
        public: void request(const DecisionRequest& request) override;
        void submit(DecisionResult result);
        std::optional<DecisionResult> poll() override;
    };
}
