#pragma once
#include "IDecisionProvider.hpp"
namespace ds {
    class HybridDecisionProvider: public IDecisionProvider {
        IDecisionProvider& proposals_;
        std::optional<DecisionResult> awaiting_;
        bool approved_=false;
        public: explicit HybridDecisionProvider(IDecisionProvider& proposals);
        void request(const DecisionRequest& request) override;
        std::optional<DecisionResult> poll() override;
        std::optional<DecisionResult> proposal() const;
        void approve(bool accepted);
    };
}
