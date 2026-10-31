#pragma once
#include "Policy.hpp"
#include "PoliticalState.hpp"
#include "Dictator.hpp"
namespace ds {
    class Government {
        public: Policy policy;
        PoliticalState state;
        Dictator leader;
        explicit Government(int tax, int welfare, double police);
    };
}
