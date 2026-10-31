#pragma once
#include "PoliticalAction.hpp"
namespace ds {
    class World;
    class PolicyExecutor {
        public: bool execute(World& world,const PoliticalAction& action) const;
    };
}
