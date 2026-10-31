#include "dictatorsim/economy/systems/ConsumptionSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void ConsumptionSystem::update(World& w) const {
        auto& seller=w.companies[0];
        w.economy.purchases=0;
        w.economy.refused=0;
        const auto& people=w.population.getPopulation();
        // Rotate first buyer to avoid permanently favoring low identifiers in shortages.
        for(std::size_t i=0;i<people.size();++i) {
            auto& p=*w.population.findCitizen(people[(i+w.clock.day())%people.size()].getId());
            bool bought=seller.sell(w.ledger,p.account,w.clock.day());
            p.foodSatisfaction=std::clamp(p.foodSatisfaction+(bought?20.f:-25.f),0.f,100.f);
            if(bought)++w.economy.purchases;
            else ++w.economy.refused;
        }
    }
}
