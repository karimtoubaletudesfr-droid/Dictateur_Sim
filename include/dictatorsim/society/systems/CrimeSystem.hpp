#pragma once
#include "dictatorsim/society/CrimeEvent.hpp"
namespace ds {
    class World;
    class CrimeSystem {
        public:
        void update(World& world) const;
        CrimeEvent attempt(World& world, std::uint32_t author, std::uint32_t victim, bool succeeds, bool reported) const;
    };
}
