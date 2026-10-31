#include "dictatorsim/core/Simulation.hpp"
#include <stdexcept>
#include "dictatorsim/economy/EconomyEngine.hpp"
#include "dictatorsim/society/SocietyEngine.hpp"
#include "dictatorsim/military/MilitaryEngine.hpp"
#include "dictatorsim/disasters/DisasterSystem.hpp"
#include "dictatorsim/politics/PolicyExecutor.hpp"
#include "dictatorsim/politics/systems/LegitimacySystem.hpp"
#include "dictatorsim/politics/systems/StabilitySystem.hpp"
#include "dictatorsim/politics/systems/CorruptionSystem.hpp"
namespace ds {
    Simulation::Simulation(SimulationConfig c):world_(c) {
        statistics_.update(world_);
    }
    void Simulation::step() {
        world_.clock.advance();
        decisions_.update(world_,report());
        DisasterSystem{}.update(world_);
        // Military wages precede taxation; loyalty/coup read previous-day political state.
        MilitaryEngine{}.update(world_);
        EconomyEngine{}.update(world_);
        CorruptionSystem{}.update(world_);
        PoliticalConsequenceSystem{}.update(world_);
        SocietyEngine{}.update(world_);
        LegitimacySystem{}.update(world_);
        StabilitySystem{}.update(world_);
        world_.ledger.flushArchive();
        world_.logger.collect(world_.events);
        statistics_.update(world_);
        world_.verify();
    }
    void Simulation::run(std::uint64_t days) {
        for(std::uint64_t i=0;i<days;++i)step();
    }
    const World& Simulation::world()const {
        return world_;
    }
    GovernmentReport Simulation::report()const {
        return {
            statistics_.history().snapshots().back()
        };
    }
    const StatisticsEngine& Simulation::statistics()const {
        return statistics_;
    }
    void Simulation::setDecisionProvider(IDecisionProvider* p) {
        if(world_.config.playerMode && p)throw std::logic_error("player mode forbids automatic decision providers");
        decisions_.setProvider(p);
    }
    bool Simulation::act(const PoliticalAction& a) {
        bool accepted=PolicyExecutor{}.execute(world_,a);
        world_.logger.collect(world_.events);
        if(accepted) {
            LegitimacySystem{}.update(world_);
            StabilitySystem{}.update(world_);
            statistics_.update(world_);
            world_.verify();
        }
        return accepted;
    }
}

namespace ds {void Simulation::acknowledgePlayerEvent(){if(world_.government.state.pendingCoup)world_.government.state.coupResponseReady=true;}}
