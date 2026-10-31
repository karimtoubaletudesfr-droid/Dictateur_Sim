#include "TestWorld.hpp"
#include "dictatorsim/economy/systems/LivingCostSystem.hpp"
int main() {
    auto c=config(1);
    c.moneySupply=0;
    ds::World w(c);
    auto& p=*w.population.findCitizen(0);
    p.pendingCost=1234;
    p.pendingService=4;
    w.companies[4].stock=1;
    auto health=p.health;
    ds::LivingCostSystem{}.update(w);
    assert(p.pendingCost==1234 && w.companies[4].stock==1 && p.health==health);
    assert(!w.ledger.transactions().empty());
    assert(w.ledger.transactions().back().status==ds::TransactionStatus::InsufficientFunds);
    w.verify();
}
