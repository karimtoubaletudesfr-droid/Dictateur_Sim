#include "dictatorsim/economy/systems/PricingSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void PricingSystem::update(World& w) const {
        if(!w.clock.monthEnd())return;
        for(auto& c:w.companies) {
            Money old=c.price;
            // Inventory feedback, bounded to one tenth / ten times the configured base price.
            Money base=c.sector==Sector::Agriculture?w.config.foodPrice:w.config.livingCost;
            Money step=std::max<Money>(1,c.price/50);
            if(c.stock<static_cast<double>(w.population.getPopulationSize()))c.price=std::min(base*10,c.price+step);
            else if(c.stock>static_cast<double>(w.population.getPopulationSize())*15)c.price=std::max<Money>(std::max<Money>(1,base/10),c.price-step);
            if(c.price!=old)w.event(EventType::PricesChanged,c.id,"Inventory feedback changed unit price from "+std::to_string(old)+" to "+std::to_string(c.price)+" cents");
        }
    }
}
