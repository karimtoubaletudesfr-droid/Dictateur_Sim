#include "dictatorsim/statistics/HistoryManager.hpp"
namespace ds {
    void HistoryManager::append(StatisticsSnapshot s) {
        snapshots_.push_back(s);
    }
    const std::vector<StatisticsSnapshot>& HistoryManager::snapshots()const {
        return snapshots_;
    }
}
