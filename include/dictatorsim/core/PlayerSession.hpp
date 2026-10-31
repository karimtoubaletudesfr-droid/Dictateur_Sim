#pragma once
#include "Simulation.hpp"
#include "dictatorsim/decisions/PlayerDecisionController.hpp"
#include "dictatorsim/events/PlayerNotification.hpp"
#include <map>
namespace ds {
enum class SimulationSpeed { Pause=0, Normal=1, Double=2, Fast=5, Fastest=10 };
class PlayerSession {
    Simulation simulation_;
    PlayerDecisionController decisions_;
    SimulationSpeed speed_=SimulationSpeed::Pause;
    double accumulator_=0;
    std::size_t eventCursor_=0;
    bool lost_=false;
    std::vector<PlayerNotification> notifications_;
    std::optional<DecisionRequest> request_;
    std::map<EventCategory,std::uint64_t> cooldown_;
    void collect();
    void activateNotification(std::size_t index);
public:
    explicit PlayerSession(SimulationConfig config);
    void advance(double elapsedSeconds);
    void setSpeed(SimulationSpeed speed);
    SimulationSpeed speed() const;
    double backlog() const;
    bool lost() const;
    const Simulation& simulation() const;
    const std::vector<PlayerNotification>& notifications() const;
    const std::optional<DecisionRequest>& request() const;
    const PlayerDecisionController& decisions() const;
    DecisionResult decide(PoliticalAction action);
    bool respond(std::size_t option);
    void notify(PlayerNotification notification);
};
}
