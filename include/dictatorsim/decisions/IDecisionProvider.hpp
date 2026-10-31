#pragma once
#include "DecisionRequest.hpp"
#include "DecisionResult.hpp"
#include <optional>
namespace ds {
    class IDecisionProvider {
        public: virtual ~IDecisionProvider()=default;
        virtual void request(const DecisionRequest& request)=0;
        virtual std::optional<DecisionResult> poll()=0;
    };
}
