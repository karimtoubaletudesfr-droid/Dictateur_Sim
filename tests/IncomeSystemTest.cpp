#include "TestWorld.hpp"
#include "dictatorsim/economy/systems/WageSystem.hpp"
#include "dictatorsim/economy/systems/IncomeSystem.hpp"
int main() {
    ds::World w(config());
    month(w);
    ds::WageSystem{}.update(w);
    bool paid=false;
    for(const auto& p:w.population.getPopulation())if(p.employer>=0) {
        assert(p.earned==w.professionConfig(p.profession).referenceSalary);
        paid=true;
    }
    assert(paid);
    auto before=w.treasury.balance(w.ledger);
    ds::IncomeSystem{}.update(w);
    assert(w.treasury.balance(w.ledger)<before);
    w.verify();
    ds::World poor(config());
    month(poor);
    for(auto& firm:poor.companies) {
        auto b=poor.ledger.balance(firm.account);
        assert(poor.ledger.transfer(firm.account,poor.treasury.account(),b,ds::TransactionType::Contract,0));
    }
    ds::WageSystem{}.update(poor);
    assert(poor.economy.wagesPaid==0);
    poor.verify();
}
