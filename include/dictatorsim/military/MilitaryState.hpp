#pragma once
#include "MilitaryUnit.hpp"
#include "dictatorsim/economy/Money.hpp"
namespace ds {
    struct MilitaryState {
        MilitaryUnit unit;
        Money monthlyBudget=0, spent=0;
        std::size_t paid=0;
        double loyalty=80, satisfaction=80;
    };
}
