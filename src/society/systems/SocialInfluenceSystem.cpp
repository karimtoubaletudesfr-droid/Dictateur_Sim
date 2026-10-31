#include "dictatorsim/society/systems/SocialInfluenceSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void SocialInfluenceSystem::update(World& w) const {
        // Snapshot first: no traversal-order cascade within one day.
        std::vector<float> trust;
        for(const auto& p:w.population.getPopulation())trust.push_back(p.governmentTrust);
        const auto& people=w.population.getPopulation();
        for(std::size_t i=0;i<people.size();++i) {
            auto& p=*w.population.findCitizen(people[i].getId());
            auto next=(i+1)%people.size();
            p.governmentTrust=static_cast<float>(.98*trust[i]+.02*trust[next]);
        }
    }
}
