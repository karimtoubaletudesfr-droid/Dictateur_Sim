#pragma once
#include "PoliticalAction.hpp"
#include <string>
namespace ds {
    class World;
    class ActionValidator {
        public: std::string validate(const World& world,const PoliticalAction& action) const;
    };
}
