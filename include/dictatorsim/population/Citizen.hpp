#pragma once

#include <cstdint>
#include "Profession.hpp"
namespace ds {

struct Citizen {
    // Identité
    std::uint32_t id;
    std::uint8_t age;
    Profession profession;
    // Situation économique
    double income;
    double savings;
    // Besoins
    float foodSatisfaction;
    float health;
    float fatigue;

    // État psychologique et politique
    float happiness;
    float governmentTrust;
    float anger;

    public:
    Citizen(
        std::uint32_t id,
        std::uint8_t age,
        Profession profession
        );
    std::uint32_t getId()const;
    std::uint8_t getAge()const;
    Profession getProfession()const;

    double getIncome()const;
    double getSavings()const;


    float getFoodSatisfaction()const;
    float getHealth()const;
    float getFatigue()const;


    float getHappiness()const;
    float getGovernmentTrust();
    float getAnger()const;


};

}