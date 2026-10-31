#include "dictatorsim/politics/systems/CorruptionSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void CorruptionSystem::update(World& w) const {
        if(w.population.getPopulationSize()==0 || !w.politicalRandom.probability(w.config.corruptionProbability))return;
        auto& recipient=w.population.getPopulation()[w.politicalRandom.randomInt(0,static_cast<int>(w.population.getPopulationSize()-1))];
        Money amount=std::min(w.config.livingCost,w.treasury.balance(w.ledger));
        if(amount>0 && w.treasury.pay(w.ledger,recipient.account,amount,TransactionType::Corruption,w.clock.day()))w.event(EventType::PaymentCompleted,recipient.getId(),"Public funds diverted; corruption transfer recorded");
    }
}
