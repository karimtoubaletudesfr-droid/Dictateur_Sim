#include "dictatorsim/society/systems/RebellionSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void RebellionSystem::update(World& w) const {
        if(w.government.state.regime==RegimeStatus::Collapsed)return;
        auto n=w.population.getPopulationSize();
        double participation=n?double(w.society.protesters)/n:0;
        double pressure=participation*(w.society.anger/100.)*(1-w.government.state.legitimacy/100.);
        if(!w.society.rebellion && participation*100 >= w.config.rebellionParticipation && w.socialRandom.probability(w.config.rebellionProbability*pressure)) {
            w.society.rebellion=true;
            w.government.state.regime=RegimeStatus::Crisis;
            w.event(EventType::RebellionStarted,0,"Rebellion emerged from mobilization, anger and low legitimacy");
        }
        if(w.society.rebellion && w.socialRandom.probability(.02)) {
            if(w.socialRandom.probability(w.military.loyalty/100.)) {
                w.society.rebellion=false;
                w.government.state.regime=RegimeStatus::Governing;
            }
            else {
                w.government.state.regime=w.socialRandom.probability(.5)?RegimeStatus::Transition:RegimeStatus::Collapsed;
                w.society.rebellion=false;
                w.event(EventType::GovernmentCollapsed,0,"Rebellion ended incumbent rule");
            }
        }
    }
}
