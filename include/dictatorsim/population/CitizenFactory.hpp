#pragma once 
#include <cstddef>
#include <cstdint>
#include <vector>

#include "dictatorsim/population/Citizen.hpp"
#include "dictatorsim/core/RandomEngine.hpp"
#include "dictatorsim/tools/IdGenerator.hpp"
#include "dictatorsim/population/ProfessionConfig.hpp"


namespace ds{
    class CitizenFactory{
        private : 
            IdGenerator& idGenerator;
            RandomEngine& randomEngine;
            std::vector<ProfessionConfig> professions;


            std::uint8_t generateAge();
            Profession generateProfession(std::uint8_t age);
        public:
        CitizenFactory(
            IdGenerator&idGenerator,
            RandomEngine& randomEngine,
            const std::vector<ProfessionConfig>& professions
        );
        Citizen createCitizen();
        std::vector<Citizen> createPopulation(std::size_t size);

    };
}
