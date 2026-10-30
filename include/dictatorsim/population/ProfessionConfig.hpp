#pragma once

#include <cstdint>

#include "dictatorsim/population/Profession.hpp"

namespace ds {

struct ProfessionConfig {
    Profession profession;

    std::uint8_t minAge;
    std::uint8_t maxAge;

    double weight;
};

}