#pragma once
#include "dictatorsim/politics/PoliticalAction.hpp"
#include <string>
namespace ds {
struct DecisionOption { std::string label, description; PoliticalAction action; bool hasAction=true; };
}
