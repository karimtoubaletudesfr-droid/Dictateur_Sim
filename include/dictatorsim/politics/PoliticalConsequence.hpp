#pragma once
#include <cstdint>
namespace ds {
struct PoliticalConsequence {
    std::uint64_t day;
    double severity;
    bool revealed=false;
};
class World;
class PoliticalConsequenceSystem { public: void update(World& world) const; };
}
