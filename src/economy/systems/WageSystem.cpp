#include "dictatorsim/economy/systems/WageSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void WageSystem::update(World& w) const {
        if(!w.clock.monthEnd())return;
        w.economy.wagesPaid=0;
        for(auto& firm:w.companies)for(auto id:firm.employees) {
            auto* p=w.population.findCitizen(id);
            if(!p)continue;
            Money salary=w.professionConfig(p->profession).referenceSalary;
            if(salary>0 && w.ledger.transfer(firm.account,p->account,salary,TransactionType::Wage,w.clock.day())) {
                p->earned=salary;
                p->income=static_cast<double>(salary)/100.;
                w.economy.wagesPaid+=salary;
            }
            else {
                p->earned=0;
                p->income=0;
                w.event(EventType::PaymentRefused,id,"Employer could not pay monthly wage");
            }
        }
    }
}
