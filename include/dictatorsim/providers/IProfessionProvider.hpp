#pragma once

#include <vector>

#include "dictatorsim/population/ProfessionConfig.hpp"

namespace ds {

class IProfessionProvider {
public:
    virtual ~IProfessionProvider() = default;

    virtual std::vector<ProfessionConfig> load() const = 0;
};

}