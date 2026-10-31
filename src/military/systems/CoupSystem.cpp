#include "dictatorsim/military/systems/CoupSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void CoupSystem::update(World& w) const {
        if(w.military.unit.citizens.empty() || w.government.state.regime==RegimeStatus::Collapsed)return;
        double risk=w.config.coupProbability*(1-w.military.loyalty/100.)*(1-w.government.state.legitimacy/100.)*(1-w.government.state.stability/100.);
        auto& state=w.government.state;
        if(state.pendingCoup && !state.coupResponseReady)return;
        if(state.pendingCoup && state.coupResponseReady) {
            state.pendingCoup=false; state.coupResponseReady=false;
            if(w.militaryRandom.probability(w.military.loyalty/100.))state.regime=RegimeStatus::Governing;
            else { ++state.leadershipChanges; state.regime=RegimeStatus::Transition; }
            return;
        }
        if(!state.pendingCoup && w.militaryRandom.probability(risk)) {
            if(w.config.playerMode) {
                state.pendingCoup=true;
                w.event(EventType::MilitaryCoupStarted,0,"Coup threat: player response required before resolution");
                return;
            }
            w.event(EventType::MilitaryCoupStarted,0,"Coup attempt after military discontent and weak legitimacy");
            double roll=w.militaryRandom.randomFloat(0,1);
            if(roll<.4) {
                ++w.government.state.leadershipChanges;
                w.government.state.regime=RegimeStatus::Transition;
            }
            else if(roll<.7)w.government.state.regime=RegimeStatus::Crisis;
            else w.government.state.regime=RegimeStatus::Governing;
        }
    }
}
