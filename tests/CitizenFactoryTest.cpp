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

int main() {

    // Chargement de la configuration
    CSVProfessionProvider provider("configs/professions.csv");
    const auto professions = provider.load();

    // Initialisation
    IdGenerator idGenerator;
    RandomEngine randomEngine(42);

    CitizenFactory factory(
        idGenerator,
        randomEngine,
        professions
    );

    // TEST 1 : génération de 10 000 citoyens
    constexpr std::size_t populationSize = 10000;

    const auto population = factory.createPopulation(populationSize);

    assert(population.size() == populationSize);

    // TEST 2 : unicité des identifiants
    std::unordered_set<std::uint32_t> ids;

    for (const auto& citizen : population) {
        assert(ids.insert(citizen.getId()).second);
    }

    // TEST 3 : compatibilité âge / profession
    for (const auto& citizen : population) {

        bool compatible = false;

        for (const auto& config : professions) {

            if (citizen.getProfession() == config.profession &&
                citizen.getAge() >= config.minAge &&
                citizen.getAge() <= config.maxAge &&
                config.weight > 0.0) {

                compatible = true;
                break;
            }
        }

        assert(compatible);
    }

    // TEST 4 : reproductibilité
    IdGenerator idGenerator2;
    RandomEngine randomEngine2(42);

    CitizenFactory factory2(
        idGenerator2,
        randomEngine2,
        professions
    );

    const auto population2 = factory2.createPopulation(populationSize);

    for (std::size_t i = 0; i < populationSize; ++i) {
        assert(population[i].getId() == population2[i].getId());
        assert(population[i].getAge() == population2[i].getAge());
        assert(population[i].getProfession() ==
               population2[i].getProfession());
    }

    // TEST 5 : population vide
    IdGenerator emptyIds;
    RandomEngine emptyRandom(123);

    CitizenFactory emptyFactory(
        emptyIds,
        emptyRandom,
        professions
    );

    assert(emptyFactory.createPopulation(0).empty());

    std::cout << "All CitizenFactory tests passed!\n";

    return 0;
}