#include "dictatorsim/economy/systems/ProductionSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void ProductionSystem::update(World& w) const {
        w.economy.totalProduction=0;
        if(w.clock.day()>w.economy.droughtUntil)w.economy.agriculturalMultiplier=1;
        for(auto& firm:w.companies) {
            double labor=0;
            for(auto id:firm.employees) {
                const auto* p=w.population.findCitizen(id);
                if(p)labor+=w.professionConfig(p->profession).productivity*(p->health/100.)*(1-p->fatigue/150.);
            }
            w.economy.totalProduction+=firm.produce(labor,firm.sector==Sector::Agriculture?w.economy.agriculturalMultiplier:1);
        }
    }
}
