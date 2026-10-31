#include "dictatorsim/politics/systems/StabilitySystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void StabilitySystem::update(World& w) const {
        auto n=w.population.getPopulationSize();
        double mobilization=n?100.*w.society.protesters/n:0;
        w.government.state.stability=std::clamp(.45*w.government.state.legitimacy+.3*w.military.loyalty+.25*(100-mobilization)-(w.society.rebellion?25:0),0.,100.);
    }
}
