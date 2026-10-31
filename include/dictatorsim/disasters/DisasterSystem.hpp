#pragma once
#include "DisasterEvent.hpp"
namespace ds {
    class World;
    class DisasterSystem {
        public: void update(World& world) const;
        void apply(World& world,const DisasterEvent& event) const;
    };
}
