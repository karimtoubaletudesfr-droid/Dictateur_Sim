#pragma once
#include "IDecisionProvider.hpp"
namespace ds {
    class World;
    class DecisionManager {
        IDecisionProvider* provider_=nullptr;
        std::uint64_t requestId_=0;
        std::uint64_t minimumResult_=0;
        public: void setProvider(IDecisionProvider* provider);
        void update(World& world,const GovernmentReport& report);
    };
}
