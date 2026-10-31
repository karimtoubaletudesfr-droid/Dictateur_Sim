#include "dictatorsim/economy/systems/InflationSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void InflationSystem::update(World& w) const {
        double previous=w.economy.priceIndex;
        double basket=0,base=0;
        for(const auto& c:w.companies) {
            basket+=static_cast<double>(c.price);
            base+=static_cast<double>(c.sector==Sector::Agriculture?w.config.foodPrice:w.config.livingCost);
        }
        w.economy.priceIndex=basket/base;
        w.economy.inflation=w.economy.priceIndex/previous-1;
    }
}
