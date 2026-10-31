#include "dictatorsim/population/PopulationManager.hpp"
#include "dictatorsim/population/CitizenFactory.hpp"
#include "dictatorsim/providers/CSVProfessionProvider.hpp"
#include "dictatorsim/core/RandomEngine.hpp"
#include "dictatorsim/tools/IdGenerator.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace ds;

// ======================================================
// Verify consistency between vector and unordered_map
// ======================================================

void checkPopulationConsistency(const PopulationManager& manager) {

    const auto& population = manager.getPopulation();

    std::unordered_set<std::uint32_t> ids;

    // Verify population size
    assert(population.size() == manager.getPopulationSize());

    for (const auto& citizen : population) {

        const std::uint32_t id = citizen.getId();

        // Every citizen must have a unique ID
        assert(ids.insert(id).second);

        // Every citizen must be accessible through the index
        const Citizen* found = manager.findCitizen(id);

        assert(found != nullptr);

        // Verify that both structures refer to the same citizen
        assert(found == &citizen);

        assert(found->getId() == id);
    }

    assert(ids.size() == manager.getPopulationSize());
}

// ======================================================
// MAIN
// ======================================================

int main() {

    CSVProfessionProvider provider("configs/professions.csv");

    const auto professions = provider.load();

    IdGenerator ids;
    RandomEngine random(42);

    CitizenFactory factory(ids, random, professions);
    PopulationManager manager(factory);

    // ==================================================
    // TEST 1: Initial population is empty
    // ==================================================

    assert(manager.getPopulationSize() == 0);
    assert(manager.getPopulation().empty());

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 2: Generate 10,000 citizens
    // ==================================================

    constexpr std::size_t populationSize = 10000;

    manager.generatePopulation(populationSize);

    assert(manager.getPopulationSize() == populationSize);

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 3: Search for an existing citizen
    // ==================================================

    const std::uint32_t existingId =
        manager.getPopulation()[500].getId();

    const Citizen* citizen = manager.findCitizen(existingId);

    assert(citizen != nullptr);
    assert(citizen->getId() == existingId);

    // ==================================================
    // TEST 4: Search for nonexistent citizen
    // ==================================================

    assert(manager.findCitizen(1000000) == nullptr);

    // ==================================================
    // TEST 5: Delete a citizen from the middle
    // ==================================================

    assert(manager.deleteCitizen(existingId));

    assert(manager.getPopulationSize() == populationSize - 1);

    assert(manager.findCitizen(existingId) == nullptr);

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 6: Delete the same citizen twice
    // ==================================================

    assert(!manager.deleteCitizen(existingId));

    assert(manager.getPopulationSize() == populationSize - 1);

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 7: Delete the first citizen
    // ==================================================

    const std::uint32_t firstId =
        manager.getPopulation().front().getId();

    assert(manager.deleteCitizen(firstId));

    assert(manager.findCitizen(firstId) == nullptr);

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 8: Delete the last citizen
    // ==================================================

    const std::uint32_t lastId =
        manager.getPopulation().back().getId();

    assert(manager.deleteCitizen(lastId));

    assert(manager.findCitizen(lastId) == nullptr);

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 9: Delete multiple citizens
    // ==================================================

    std::vector<std::uint32_t> idsToDelete;

    // Collect IDs before modifying the vector
    for (std::size_t i = 0; i < 100; ++i) {
        idsToDelete.push_back(
            manager.getPopulation()[i].getId()
        );
    }

    for (const auto id : idsToDelete) {
        assert(manager.deleteCitizen(id));
        assert(manager.findCitizen(id) == nullptr);
    }

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 10: Regenerate the population
    // ==================================================

    manager.generatePopulation(100);

    assert(manager.getPopulationSize() == 100);

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 11: Generate an empty population
    // ==================================================

    manager.generatePopulation(0);

    assert(manager.getPopulationSize() == 0);
    assert(manager.getPopulation().empty());

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 12: Delete from an empty population
    // ==================================================

    assert(!manager.deleteCitizen(42));

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 13: Delete every citizen
    // ==================================================

    manager.generatePopulation(1000);

    std::vector<std::uint32_t> allIds;

    allIds.reserve(manager.getPopulationSize());

    for (const auto& currentCitizen : manager.getPopulation()) {
        allIds.push_back(currentCitizen.getId());
    }

    for (const auto id : allIds) {
        assert(manager.deleteCitizen(id));
    }

    assert(manager.getPopulationSize() == 0);
    assert(manager.getPopulation().empty());

    // All deleted IDs must remain inaccessible
    for (const auto id : allIds) {
        assert(manager.findCitizen(id) == nullptr);
    }

    checkPopulationConsistency(manager);

    // ==================================================
    // TEST 14: Generate population after full deletion
    // ==================================================

    manager.generatePopulation(500);

    assert(manager.getPopulationSize() == 500);

    checkPopulationConsistency(manager);

    std::cout<< " All PopulationManager tests passed!\n";

    return 0;
}