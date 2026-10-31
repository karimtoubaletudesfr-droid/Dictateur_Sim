#include "dictatorsim/economy/systems/IncomeSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void IncomeSystem::update(World& w) const {
        if(!w.clock.monthEnd())return;
        // Means-tested relief is distributed from a fixed fraction of existing public funds.
        std::size_t eligible=0;
        for(const auto& p:w.population.getPopulation())if(p.employer<0 && p.profession!=Profession::Soldier)++eligible;
        Money pool=fraction(w.treasury.balance(w.ledger),w.government.policy.welfareRate);
        Money payment=eligible?pool/static_cast<Money>(eligible):0;
        if(payment>0)for(const auto& p:w.population.getPopulation())if(p.employer<0 && p.profession!=Profession::Soldier)w.treasury.pay(w.ledger,p.account,payment,TransactionType::Subsidy,w.clock.day());
    }
}
