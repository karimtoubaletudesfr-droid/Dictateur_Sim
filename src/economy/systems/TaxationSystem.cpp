#include "dictatorsim/economy/systems/TaxationSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void TaxationSystem::update(World& w) const {
        if(!w.clock.monthEnd())return;
        for(const auto& item:w.population.getPopulation()) {
            auto& p=*w.population.findCitizen(item.getId());
            Money tax=fraction(p.earned,w.government.policy.incomeTax);
            if(tax>0)w.ledger.transfer(p.account,w.treasury.account(),tax,TransactionType::Tax,w.clock.day());
            p.earned=0;
        }
    }
}
