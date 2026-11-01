#pragma once
#include "World.hpp"
#include "dictatorsim/statistics/StatisticsEngine.hpp"
#include "dictatorsim/decisions/DecisionManager.hpp"
namespace ds {
    class Simulation {
        World world_;
        StatisticsEngine statistics_;
        DecisionManager decisions_;
        public:
        explicit Simulation(SimulationConfig config);
        void step();
        void applySettings(const SimulationConfig& settings);
        void run(std::uint64_t days);
        const World& world() const;
        GovernmentReport report() const;
        const StatisticsEngine& statistics() const;
        void setDecisionProvider(IDecisionProvider* provider);
        bool act(const PoliticalAction& action);
        void acknowledgePlayerEvent();
    };
}
