#pragma once
#include "dictatorsim/core/World.hpp"
#include <cassert>
inline ds::SimulationConfig config(std::size_t n=100) {
    ds::SimulationConfig c;
    c.population=n;
    c.breakdownProbability=0;
    c.sicknessProbability=0;
    c.disasterProbability=0;
    c.crimeProbability=0;
    c.corruptionProbability=0;
    return c;
}
inline void month(ds::World& w) {
    for(int i=0;i<30;++i)w.clock.advance();
}
