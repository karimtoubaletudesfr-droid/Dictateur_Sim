#include "dictatorsim/military/systems/RecruitmentSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void RecruitmentSystem::update(World& w) const {
        // Reconcile identifiers with civilian population; soldiers are never copied.
        auto& ids=w.military.unit.citizens;
        ids.clear();
        for(const auto& p:w.population.getPopulation())if(p.profession==Profession::Soldier)ids.push_back(p.getId());
    }
}
