#include "dictatorsim/military/systems/LoyaltySystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void LoyaltySystem::update(World& w) const {
        if(w.military.unit.citizens.empty()) {
            w.military.loyalty=0;
            w.military.satisfaction=0;
            return;
        }
        if(w.clock.monthEnd()) {
            w.military.paid=0;
            w.military.spent=0;
            for(auto id:w.military.unit.citizens) {
                auto& p=*w.population.findCitizen(id);
                if(w.config.salaries[10]<=w.military.monthlyBudget-w.military.spent && w.treasury.pay(w.ledger,p.account,w.config.salaries[10],TransactionType::Wage,w.clock.day())) {
                    w.military.spent+=w.config.salaries[10];
                    p.earned=w.config.salaries[10];
                    ++w.military.paid;
                }
                else w.event(EventType::PaymentRefused,id,"Military salary unpaid: treasury or appropriation insufficient");
            }
            w.military.satisfaction=100.*w.military.paid/w.military.unit.citizens.size();
        }
        double conditions=0;
        for(auto id:w.military.unit.citizens)conditions+=w.population.findCitizen(id)->happiness;
        conditions/=w.military.unit.citizens.size();
        double target=.5*w.military.satisfaction+.25*conditions+.25*w.government.state.legitimacy;
        w.military.loyalty=std::clamp(.98*w.military.loyalty+.02*target,0.,100.);
    }
}
