#include "TestWorld.hpp"
#include "dictatorsim/military/MilitaryEngine.hpp"
int main() {
    ds::World w(config(1000));
    month(w);
    w.military.monthlyBudget=w.treasury.balance(w.ledger);
    auto before=w.treasury.balance(w.ledger);
    ds::MilitaryEngine{}.update(w);
    assert(!w.military.unit.citizens.empty());
    assert(w.military.paid==w.military.unit.citizens.size());
    assert(w.treasury.balance(w.ledger)==before-w.military.spent);
    auto id=w.military.unit.citizens.front();
    assert(w.population.findCitizen(id)->earned==w.config.wage);
    w.verify();
    ds::World empty(config(0));
    ds::MilitaryEngine{}.update(empty);
    assert(empty.military.loyalty==0);
}
