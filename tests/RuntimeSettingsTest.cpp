#include "TestWorld.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
#include "dictatorsim/core/RuntimeSettings.hpp"
#include "dictatorsim/economy/systems/WageSystem.hpp"
#include "dictatorsim/society/systems/RebellionSystem.hpp"
#include "dictatorsim/military/systems/CoupSystem.hpp"
#include <limits>
int main(){
    ds::World w(config(200));auto settings=w.config;
    auto supply=w.ledger.moneySupply();
    settings.salaries[3]=12345;settings.salaries[10]=0;settings.crimeProbability=.12;
    settings.rebellionParticipation=90;settings.coupLoyaltyThreshold=0;
    settings.population=999; // Live settings cannot replace population or initial funds.
    settings.moneySupply=1;
    w.applySettings(settings);
    assert(w.population.getPopulationSize()==200 && w.config.population==200);
    assert(w.config.crimeProbability==.12 && w.professionConfig(ds::Profession::Farmer).referenceSalary==12345);
    assert(w.ledger.moneySupply()==supply && w.ledger.conserved());
    month(w);ds::WageSystem{}.update(w);
    bool paid=false;for(const auto& citizen:w.population.getPopulation())if(citizen.profession==ds::Profession::Farmer){assert(citizen.earned==12345);paid=true;}
    assert(paid);
    settings=w.config;settings.salaries[3]=54321;settings.disasterProbability=std::numeric_limits<double>::quiet_NaN();
    bool refused=false;try{w.applySettings(settings);}catch(const std::invalid_argument&){refused=true;}
    assert(refused && w.professionConfig(ds::Profession::Farmer).referenceSalary==12345);
    w.config.rebellionProbability=1;w.society.protesters=100;w.society.anger=100;w.government.state.legitimacy=0;
    for(int i=0;i<100;++i)ds::RebellionSystem{}.update(w);
    assert(!w.society.rebellion);
    w.config.coupProbability=1;w.military.loyalty=1;w.government.state.stability=0;
    for(int i=0;i<100;++i)ds::CoupSystem{}.update(w);
    assert(w.government.state.leadershipChanges==0);
    ds::PlayerSession session(config(100));auto changed=session.simulation().world().config;changed.salaries[4]=777;
    session.setSpeed(ds::SimulationSpeed::Normal);session.applySettings(changed);
    assert(session.speed()==ds::SimulationSpeed::Pause && session.simulation().world().clock.day()==0);
    changed.population=17;ds::PlayerSession restarted(changed);
    assert(restarted.simulation().world().population.getPopulationSize()==17);
    assert(restarted.simulation().world().professionConfig(ds::Profession::Baker).referenceSalary==777);
}
