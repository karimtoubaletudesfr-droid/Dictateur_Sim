#include "dictatorsim/population/PopulationManager.hpp"

#include <utility>

namespace ds {


// Constructor


PopulationManager::PopulationManager(CitizenFactory& factory)
    : factory(factory)
{
}


// Population generation


void PopulationManager::generatePopulation(std::size_t size) {

    population = factory.createPopulation(size);

    citizenIndex.clear();
    citizenIndex.reserve(population.size());

    for (std::size_t i = 0; i < population.size(); ++i) {
        citizenIndex.emplace(population[i].getId(), i);
    }
}


// Population access


const std::vector<Citizen>& PopulationManager::getPopulation() const {
    return population;
}

std::size_t PopulationManager::getPopulationSize() const {
    return population.size();
}


// Citizen lookup


const Citizen* PopulationManager::findCitizen(std::uint32_t id) const {

    auto it = citizenIndex.find(id);

    if (it == citizenIndex.end()) {
        return nullptr;
    }

    return &population[it->second];
}


// Citizen deletion (swap-and-pop)


bool PopulationManager::deleteCitizen(std::uint32_t id) {
    auto it = citizenIndex.find(id);

    if (it == citizenIndex.end()) {
        return false;
    }

    const std::size_t index = it->second;
    const std::size_t lastIndex = population.size() - 1;

    if (index != lastIndex) {
        const std::uint32_t movedId = population[lastIndex].getId();

        population[index] = std::move(population[lastIndex]);

        citizenIndex.at(movedId) = index;
    }

    population.pop_back();
    citizenIndex.erase(id);

    return true;
}
}