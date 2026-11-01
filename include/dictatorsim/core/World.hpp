#pragma once
#include "SimulationConfig.hpp"
#include "dictatorsim/politics/PoliticalConsequence.hpp"
#include "SimulationClock.hpp"
#include "dictatorsim/population/PopulationManager.hpp"
#include "dictatorsim/economy/Treasury.hpp"
#include "dictatorsim/economy/Company.hpp"
#include "dictatorsim/economy/EconomicState.hpp"
#include "dictatorsim/events/EventLogger.hpp"
#include "dictatorsim/society/SocialState.hpp"
#include "dictatorsim/society/CrimeEvent.hpp"
#include "dictatorsim/politics/Government.hpp"
#include "dictatorsim/military/MilitaryState.hpp"
#include "dictatorsim/disasters/DisasterEvent.hpp"
namespace ds {
    class World {
        IdGenerator ids_;
        RandomEngine populationRandom_;
        std::vector<ProfessionConfig> professions_;
        CitizenFactory factory_;
        public:
        SimulationConfig config;
        PopulationManager population;
        Ledger ledger;
        Treasury treasury;
        std::vector<Company> companies;
        SimulationClock clock;
        RandomEngine economyRandom, socialRandom, militaryRandom, disasterRandom, politicalRandom;
        EconomicState economy;
        SocialState society;
        Government government;
        MilitaryState military;
        EventBus events;
        EventLogger logger;
        std::vector<PoliticalConsequence> politicalConsequences;
        std::vector<CrimeEvent> crimes;
        std::vector<DisasterEvent> disasters;
        explicit World(SimulationConfig config);
        World(const World&)=delete;
        World& operator=(const World&)=delete;
        void event(EventType type, std::uint32_t subject, const std::string& message);
        void verify() const;
        void applySettings(const SimulationConfig& settings);
        const ProfessionConfig& professionConfig(Profession profession) const;
    };
}
