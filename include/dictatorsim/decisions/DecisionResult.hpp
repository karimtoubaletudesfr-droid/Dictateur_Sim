#pragma once
#include "dictatorsim/politics/PoliticalAction.hpp"
#include <string>
namespace ds {
    struct DecisionResult {
        std::uint64_t requestId;
        PoliticalAction action;
        bool approved=true;
        std::string message="";
    };
}
