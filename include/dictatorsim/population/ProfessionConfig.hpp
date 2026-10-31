#pragma once

#include <cstdint>
#include "dictatorsim/economy/Resource.hpp"
#include "dictatorsim/economy/Money.hpp"

#include "dictatorsim/population/Profession.hpp"

namespace ds {

struct ProfessionConfig {
    Profession profession;

    std::uint8_t minAge;
    std::uint8_t maxAge;

    double weight;
    Money referenceSalary=60000;
    Sector sector=Sector::Commerce;
    double productivity=1, physicalLoad=1, mentalLoad=1, workHours=8, skill=0;

};

}