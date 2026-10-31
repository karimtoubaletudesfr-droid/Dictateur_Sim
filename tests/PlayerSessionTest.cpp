#include "TestWorld.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
#include "dictatorsim/decisions/HumanDecisionProvider.hpp"
int main(){
    auto c=config(100);c.coupProbability=0;c.rebellionProbability=0;
    ds::PlayerSession s(c);s.advance(10);assert(s.simulation().world().clock.day()==0);
    s.setSpeed(ds::SimulationSpeed::Normal);s.advance(.25);assert(s.simulation().world().clock.day()==0);s.advance(.25);assert(s.simulation().world().clock.day()==1);
    s.setSpeed(ds::SimulationSpeed::Double);s.advance(.25);assert(s.simulation().world().clock.day()==2);
    s.setSpeed(ds::SimulationSpeed::Fast);s.advance(.1);assert(s.simulation().world().clock.day()==3);
    s.setSpeed(ds::SimulationSpeed::Fastest);s.advance(2);assert(s.simulation().world().clock.day()==7 && s.backlog()>0);
    s.setSpeed(ds::SimulationSpeed::Pause);auto backlog=s.backlog();s.advance(100);assert(s.simulation().world().clock.day()==7 && s.backlog()==backlog);
    int tax=s.simulation().world().government.policy.incomeTax;
    assert(s.decisions().history().records().empty());assert(tax==c.taxRate);
    auto result=s.decide({ds::ActionType::IncreaseTaxes,0,500});assert(result.approved);assert(s.simulation().world().government.policy.incomeTax==tax+500);
    assert(!s.decide({ds::ActionType::SubsidizeFood,c.moneySupply+1}).approved);assert(s.simulation().world().ledger.conserved());
    s.notify({0,7,ds::EventSeverity::Major,ds::EventCategory::Disaster,"Severe drought",true,false});
    assert(s.request() && s.speed()==ds::SimulationSpeed::Pause);s.setSpeed(ds::SimulationSpeed::Fastest);s.advance(10);assert(s.simulation().world().clock.day()==7);
    assert(s.respond(s.request()->options.size()-1));assert(!s.request() && s.speed()==ds::SimulationSpeed::Pause);
    s.notify({0,8,ds::EventSeverity::Major,ds::EventCategory::Disaster,"Drought again",true,false});assert(!s.request());
    s.notify({0,8,ds::EventSeverity::Critical,ds::EventCategory::Politics,"Critical A",false,false});
    s.notify({0,8,ds::EventSeverity::Critical,ds::EventCategory::Military,"Critical B",true,false});
    assert(s.request());assert(s.respond(s.request()->options.size()-1));assert(s.request());assert(s.respond(s.request()->options.size()-1));assert(!s.request());
    auto poorConfig=c;poorConfig.moneySupply=0;ds::PlayerSession poor(poorConfig);
    poor.notify({0,0,ds::EventSeverity::Major,ds::EventCategory::Disaster,"Damage",true,false});
    assert(!poor.respond(0) && poor.request());assert(poor.respond(poor.request()->options.size()-1));assert(poor.speed()==ds::SimulationSpeed::Pause);
    ds::Simulation manual(c);ds::HumanDecisionProvider human;c.playerMode=true;ds::Simulation restricted(c);bool refused=false;
    try{restricted.setDecisionProvider(&human);}catch(const std::logic_error&){refused=true;}assert(refused);
}
