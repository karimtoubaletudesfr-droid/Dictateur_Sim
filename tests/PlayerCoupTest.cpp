#include "TestWorld.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
#include "dictatorsim/military/systems/CoupSystem.hpp"
int main(){auto c=config(1000);c.playerMode=true;c.coupProbability=1;ds::World w(c);w.military.loyalty=0;w.government.state.legitimacy=0;w.government.state.stability=0;
    ds::CoupSystem{}.update(w);assert(w.government.state.pendingCoup);assert(w.government.state.leadershipChanges==0);
    ds::CoupSystem{}.update(w);assert(w.government.state.pendingCoup && w.government.state.leadershipChanges==0);
    w.government.state.coupResponseReady=true;ds::CoupSystem{}.update(w);assert(w.government.state.regime==ds::RegimeStatus::Transition && w.government.state.leadershipChanges==1);w.verify();
    auto failed=config(1000);failed.moneySupply=0;failed.coupProbability=1;ds::PlayerSession game(failed);
    for(int i=0;i<720 && !game.lost();++i){
        while(game.request())assert(game.respond(game.request()->options.size()-1));
        game.setSpeed(ds::SimulationSpeed::Fastest);game.advance(.05);
    }
    assert(game.lost());auto day=game.simulation().world().clock.day();game.setSpeed(ds::SimulationSpeed::Fastest);game.advance(10);
    assert(game.simulation().world().clock.day()==day);assert(!game.decide({ds::ActionType::IncreaseTaxes,0,100}).approved);
}
