#include "TestWorld.hpp"
#include "dictatorsim/society/systems/CrimeSystem.hpp"
int main() {
    ds::World w(config(2));
    auto before=w.ledger.balance(w.population.findCitizen(1)->account);
    auto e=ds::CrimeSystem{}.attempt(w,0,1,true,false);
    assert(e.success && !e.reported);
    assert(w.ledger.balance(w.population.findCitizen(1)->account)==before-w.config.repairCost);
    auto second=ds::CrimeSystem{}.attempt(w,0,1,true,true);
    assert(second.restituted && second.sanctioned);
    assert(w.society.actualCrimes==2 && w.society.reportedCrimes==1);
    assert(!ds::CrimeSystem{}.attempt(w,0,1,false,false).success);
    w.verify();
}
