#include "dictatorsim/economy/systems/LivingCostSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void LivingCostSystem::update(World& w) const {
        for(const auto& item:w.population.getPopulation()) {
            auto& p=*w.population.findCitizen(item.getId());
            bool housed=true;
            // Four distinct suppliers: housing, energy, transport goods and other services.
            for(int sector:{3,1,2,5}) {
                bool paid=w.companies[sector].sell(w.ledger,p.account,w.clock.day());
                if(sector==3)housed=paid;
            }
            p.housing=std::clamp(p.housing+(housed?10.f:-10.f),0.f,100.f);
            if(p.pendingCost==0) {
                if(w.economyRandom.probability(w.config.breakdownProbability)) {
                    p.pendingCost=w.config.repairCost;
                    p.pendingService=2;
                    w.event(EventType::CarBreakdown,p.getId(),"Car breakdown; repair remains pending until paid");
                }
                else if(w.economyRandom.probability(w.config.sicknessProbability)) {
                    p.pendingCost=w.config.medicalCost;
                    p.pendingService=4;
                    p.health=std::max(0.f,p.health-20);
                    w.event(EventType::CitizenBecameSick,p.getId(),"Illness; medical treatment requested");
                }
            }
            if(p.pendingCost>0) {
                auto& supplier=w.companies.at(p.pendingService);
                auto kind=p.pendingService==4?TransactionType::Medical:TransactionType::Repair;
                if(supplier.stock>=1 && w.ledger.transfer(p.account,supplier.account,p.pendingCost,kind,w.clock.day())) {
                    supplier.stock-=1;
                    p.pendingCost=0;
                    if(p.pendingService==3)p.housing=std::min(100.f,p.housing+60);
                    if(p.pendingService==4)p.health=std::min(100.f,p.health+15);
                    w.event(EventType::PaymentCompleted,p.getId(),"Pending service paid and delivered");
                }
            }
        }
    }
}
