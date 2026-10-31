#include "dictatorsim/statistics/StatisticsEngine.hpp"
#include "dictatorsim/statistics/MetricsCalculator.hpp"
#include "dictatorsim/core/World.hpp"
namespace ds {
    void StatisticsEngine::update(World& w) {
        auto s=MetricsCalculator{}.calculate(w);
        w.economy.prosperity=s.economicProsperity;
        history_.append(s);
    }
    const HistoryManager& StatisticsEngine::history()const {
        return history_;
    }
}
