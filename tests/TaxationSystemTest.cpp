#include "TestWorld.hpp"
#include "dictatorsim/economy/systems/TaxationSystem.hpp"
int main() {
    ds::World w(config(1));
    month(w);
    auto& p=*w.population.findCitizen(0);
    p.earned=10001;
    auto before=w.treasury.balance(w.ledger);
    ds::TaxationSystem{}.update(w);
    assert(w.treasury.balance(w.ledger)==before+1500);
    assert(p.earned==0);
    ds::TaxationSystem{}.update(w);
    assert(w.treasury.balance(w.ledger)==before+1500);
    w.verify();
}
