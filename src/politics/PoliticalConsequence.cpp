#include "dictatorsim/politics/PoliticalConsequence.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
namespace ds {
void PoliticalConsequenceSystem::update(World& w) const {
    auto& state=w.government.state;
    state.fear=std::max(0.,state.fear-.05);
    state.radicalization=std::max(0.,state.radicalization-.01);
    // A reproducible abstract exposure model; no operational concealment mechanics.
    for(auto& consequence:w.politicalConsequences) {
        if(consequence.revealed || consequence.day>=w.clock.day())continue;
        double probability=std::min(1.,.003*(1+consequence.severity/10)*(1-.5*w.government.policy.police));
        if(w.politicalRandom.probability(probability)) {
            consequence.revealed=true; ++state.scandals;
            state.radicalization=std::min(100.,state.radicalization+consequence.severity);
            for(const auto& item:w.population.getPopulation()) {
                auto& p=*w.population.findCitizen(item.getId());
                p.governmentTrust=std::max(0.f,p.governmentTrust-static_cast<float>(consequence.severity));
                p.anger=std::min(100.f,p.anger+static_cast<float>(consequence.severity));
            }
            w.event(EventType::PoliticalScandal,0,"A covert state action was exposed; public inquiry and outrage");
        }
    }
}
}
