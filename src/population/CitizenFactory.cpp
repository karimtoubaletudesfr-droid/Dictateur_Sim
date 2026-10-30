#include "dictatorsim/population/CitizenFactory.hpp"
#include <stdexcept>
#include <utility>
namespace ds{
    CitizenFactory::CitizenFactory(
        IdGenerator&idGenerator,
        RandomEngine& randomEngine,
        const std::vector<ProfessionConfig>& professions
    )
        :idGenerator(idGenerator),randomEngine(randomEngine),professions(std::move(professions))
    {
        if(this->professions.empty()){
            throw std::invalid_argument(
                "Profession configurations cannot be empty"
            );
        }
    };
    std::uint8_t CitizenFactory::generateAge() {
        return static_cast<std::uint8_t>(
            randomEngine.randomInt(0, 90)
        );
    }
    Profession CitizenFactory::generateProfession(std::uint8_t age){
  
        std::vector<Profession> eligibleProfessions;
        std::vector<double> weights;
        for( const auto& config: professions){
                if(age>=config.minAge && age<=config.maxAge){
                    eligibleProfessions.push_back(config.profession);
                    weights.push_back(config.weight);
                }  
        }
       size_t professionIndex=randomEngine.weightedChoice(weights);

       return eligibleProfessions[professionIndex];
    }
    Citizen CitizenFactory::createCitizen(){
        const std::uint8_t age=generateAge();
        const Profession profession = generateProfession(age);
        return Citizen(
            idGenerator.generate(),
            age,
            profession
        );
    }; 
    std::vector<Citizen> CitizenFactory::createPopulation(std::size_t size){
        std::vector<Citizen> population;
        population.reserve(size);


        for (std::size_t i = 0; i < size; ++i) {

            population.push_back(createCitizen());

        }
        return population;

    }
}