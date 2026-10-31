#include "dictatorsim/politics/systems/LegitimacySystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void LegitimacySystem::update(World& w) const {
        double trust=0;
        for(const auto& p:w.population.getPopulation())trust+=p.governmentTrust;
        auto n=w.population.getPopulationSize();
        w.government.state.legitimacy=n?trust/n:0;
    }
}
