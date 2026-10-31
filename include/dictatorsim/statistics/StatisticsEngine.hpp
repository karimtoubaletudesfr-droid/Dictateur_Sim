#pragma once
#include "HistoryManager.hpp"
namespace ds {
    class World;
    class StatisticsEngine {
        HistoryManager history_;
        public: void update(World& world);
        const HistoryManager& history() const;
    };
}
