#include "TestWorld.hpp"
#include "dictatorsim/economy/systems/ConsumptionSystem.hpp"
#include "dictatorsim/economy/systems/LivingCostSystem.hpp"
int main() {
    ds::World w(config(1));
    auto id=w.population.getPopulation()[0].getId();
    auto& p=*w.population.findCitizen(id);
    w.companies[0].stock=2;
    auto before=w.ledger.balance(p.account);
    ds::ConsumptionSystem{}.update(w);
    assert(w.ledger.balance(p.account)==before-w.config.foodPrice);
    assert(w.companies[0].stock==1);
    for(int service:{2,4}) {
        for(auto& c:w.companies)c.stock=0;
        w.companies[service].stock=2;
        p.pendingCost=1234;
        p.pendingService=service;
        auto balance=w.ledger.balance(w.companies[service].account);
        ds::LivingCostSystem{}.update(w);
        assert(p.pendingCost==0);
        assert(w.ledger.balance(w.companies[service].account)==balance+1234+w.config.livingCost*(service==2));
    }
    w.verify();
}
