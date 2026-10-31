#include "TestWorld.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
#include "dictatorsim/politics/PolicyExecutor.hpp"
#include "dictatorsim/economy/systems/LivingCostSystem.hpp"
#include "dictatorsim/economy/systems/TaxationSystem.hpp"
int main(){
    auto c=config(50);c.coupProbability=0;c.rebellionProbability=0;
    ds::PlayerSession a(c),b(c);
    assert(a.decide({ds::ActionType::IncreaseTaxes,0,500}).approved);assert(b.decide({ds::ActionType::IncreaseTaxes,0,500}).approved);
    a.setSpeed(ds::SimulationSpeed::Normal);b.setSpeed(ds::SimulationSpeed::Normal);
    for(int i=0;i<31;++i){a.advance(.5);b.advance(.5);}
    auto& record=a.decisions().history().records().front();assert(record.afterThirtyDays);assert(record.afterThirtyDays->day==30);
    assert(a.simulation().report().observed.treasuryBalance==b.simulation().report().observed.treasuryBalance);assert(a.simulation().world().ledger.conserved());
    ds::World taxed(c),base(c);month(taxed);month(base);
    taxed.population.findCitizen(0)->earned=10000;base.population.findCitizen(0)->earned=10000;
    assert(ds::PolicyExecutor{}.execute(taxed,{ds::ActionType::IncreaseTaxes,0,500}));
    ds::TaxationSystem{}.update(taxed);ds::TaxationSystem{}.update(base);
    assert(taxed.treasury.balance(taxed.ledger)==base.treasury.balance(base.ledger)+500);
    ds::World health(c);auto& p=*health.population.findCitizen(0);p.health=50;p.pendingCost=health.config.medicalCost;p.pendingService=4;health.companies[4].stock=0;
    auto before=health.treasury.balance(health.ledger);auto productivity=health.companies[4].productivity;
    assert(ds::PolicyExecutor{}.execute(health,{ds::ActionType::IncreaseHealthcareBudget,100000}));
    assert(health.treasury.balance(health.ledger)==before-100000 && health.companies[4].productivity>productivity);
    ds::LivingCostSystem{}.update(health);assert(p.health>50 && p.pendingCost==0);health.verify();
}
