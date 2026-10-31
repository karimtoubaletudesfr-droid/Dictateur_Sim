#include "TestWorld.hpp"
#include "dictatorsim/disasters/DisasterSystem.hpp"
int main() {
    ds::World w(config());
    for(auto& c:w.companies)c.stock=100;
    auto before=w.ledger.moneySupply();
    ds::DisasterSystem{}.apply(w,{ds::DisasterType::Earthquake,0,.5,0});
    assert(w.companies[0].stock==50 && w.companies[0].capacity==.5);
    assert(w.ledger.moneySupply()==before && w.ledger.transactions().empty());
    assert(w.population.findCitizen(0)->pendingCost>0);
    ds::DisasterSystem{}.apply(w,{ds::DisasterType::Drought,1,.4,0});
    assert(w.economy.agriculturalMultiplier==.6);
    w.verify();
}
