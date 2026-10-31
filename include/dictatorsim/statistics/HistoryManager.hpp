#pragma once
#include "StatisticsSnapshot.hpp"
#include <vector>
namespace ds {
    class HistoryManager {
        std::vector<StatisticsSnapshot> snapshots_;
        public: void append(StatisticsSnapshot snapshot);
        const std::vector<StatisticsSnapshot>& snapshots() const;
    };
}
