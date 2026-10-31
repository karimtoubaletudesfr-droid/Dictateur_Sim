#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "dictatorsim/population/Citizen.hpp"
#include "dictatorsim/population/CitizenFactory.hpp"

namespace ds {

class PopulationManager {
private:
    std::vector<Citizen> population;

    // Citizen ID -> position in vector
    std::unordered_map<std::uint32_t, std::size_t> citizenIndex;

    CitizenFactory& factory;

public:
    explicit PopulationManager(CitizenFactory& factory);

    // Population generation
    void generatePopulation(std::size_t size);

    // Population access
    const std::vector<Citizen>& getPopulation() const;

    std::size_t getPopulationSize() const;

    // Citizen lookup
    const Citizen* findCitizen(std::uint32_t id) const;
    Citizen* findCitizen(std::uint32_t id);

    // Citizen deletion
    bool deleteCitizen(std::uint32_t id);
};

} // namespace ds