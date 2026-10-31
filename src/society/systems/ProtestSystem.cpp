#include "dictatorsim/society/systems/ProtestSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void ProtestSystem::update(World& w) const {
        std::size_t count=0;
        auto previous=w.society.protesters;
        for(const auto& item:w.population.getPopulation()) {
            auto& p=*w.population.findCitizen(item.getId());
            double social=w.population.getPopulationSize()?static_cast<double>(previous)/w.population.getPopulationSize():0;
            double motive=(p.anger/100.)*(1-p.governmentTrust/100.)*(.5+social);
            double risk=(w.government.policy.emergency?.25:.7)*(1-w.government.state.fear/150.);
            motive*=1+w.government.state.radicalization/100.;
            if(!p.protesting)p.protesting=w.socialRandom.probability(std::clamp(w.config.protestProbability*motive*risk,0.,1.));
            else if(w.socialRandom.probability(.03+.1*p.governmentTrust/100.))p.protesting=false;
            if(p.protesting)++count;
        }
        w.society.protesters=count;
        if(previous==0 && count>0)w.event(EventType::ProtestStarted,0,"Citizens began protesting after individual risk and trust decisions");
    }
}
