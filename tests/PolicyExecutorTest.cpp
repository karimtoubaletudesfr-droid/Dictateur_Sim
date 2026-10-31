#include "TestWorld.hpp"
#include "dictatorsim/politics/PolicyExecutor.hpp"
int main() {
    ds::World w(config());
    ds::PolicyExecutor ex;
    auto before=w.treasury.balance(w.ledger);
    assert(!ex.execute(w,{ds::ActionType::BuildHospital,before+1,0}));
    assert(w.treasury.balance(w.ledger)==before);
    assert(ex.execute(w,{ds::ActionType::IncreaseTaxes,0,100}));
    assert(w.government.policy.incomeTax==1600);
    assert(ex.execute(w,{ds::ActionType::ProvideDisasterRelief,10000,0}));
    assert(w.treasury.balance(w.ledger)==before-10000);
    assert(!ex.execute(w,{ds::ActionType::BuildHospital,w.companies[3].price,0}));
    w.companies[3].stock=1;
    assert(ex.execute(w,{ds::ActionType::BuildHospital,w.companies[3].price,0}));
    assert(w.companies[3].stock==0);
    w.verify();
}
