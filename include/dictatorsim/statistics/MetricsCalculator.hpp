#pragma once
#include "StatisticsSnapshot.hpp"
namespace ds {
    class World;
    class MetricsCalculator {
        public: StatisticsSnapshot calculate(const World& world) const;
    };
}
