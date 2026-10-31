#include "TestWorld.hpp"
#include "dictatorsim/core/Simulation.hpp"
#include "dictatorsim/decisions/HumanDecisionProvider.hpp"
int main() {
    auto c=config(40);
    c.disasterProbability=.01;
    c.crimeProbability=.02;
    c.breakdownProbability=.01;
    c.sicknessProbability=.01;
    ds::Simulation a(c),b(c);
    a.run(1080);
    b.run(1080);
    assert(a.world().ledger.moneySupply()==c.moneySupply);
    auto x=a.report().observed,y=b.report().observed;
    assert(x.averageHappiness==y.averageHappiness && x.treasuryBalance==y.treasuryBalance && x.activeProtesters==y.activeProtesters);
    auto& ta=a.world().ledger.transactions();
    auto& tb=b.world().ledger.transactions();
    assert(ta.size()==tb.size());
    for(std::size_t i=0;i<ta.size();++i) {
        assert(ta[i].amount==tb[i].amount && ta[i].source==tb[i].source && ta[i].destination==tb[i].destination && ta[i].status==tb[i].status && ta[i].type==tb[i].type);
    }
    auto d=c;
    d.crimeProbability=0;
    ds::Simulation other(d);
    other.run(1080);
    auto& da=a.world().disasters;
    auto& db=other.world().disasters;
    assert(da.size()==db.size());
    for(std::size_t i=0;i<da.size();++i)assert(da[i].day==db[i].day && da[i].severity==db[i].severity && da[i].type==db[i].type);
    ds::Simulation empty(config(0));
    empty.run(720);
    empty.world().verify();
    auto broke=config(20);
    broke.moneySupply=0;
    ds::Simulation poor(broke);
    poor.run(720);
    assert(poor.report().observed.moneySupply==0);
    ds::HumanDecisionProvider human;
    ds::Simulation controlled(config());
    controlled.setDecisionProvider(&human);
    human.submit({0,{ds::ActionType::IncreaseTaxes,0,100},true});
    controlled.step();
    assert(controlled.world().government.policy.incomeTax==1600);
}
