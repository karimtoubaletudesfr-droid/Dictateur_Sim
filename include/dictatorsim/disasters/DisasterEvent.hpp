#pragma once
#include "DisasterType.hpp"
#include <cstdint>
namespace ds {
    struct DisasterEvent {
        DisasterType type;
        unsigned region;
        double severity;
        std::uint64_t day;
    };
}
