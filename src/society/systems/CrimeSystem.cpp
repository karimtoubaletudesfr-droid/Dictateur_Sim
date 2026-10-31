#include "dictatorsim/society/systems/CrimeSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <stdexcept>
namespace ds {
    CrimeEvent CrimeSystem::attempt(World& w,std::uint32_t author,std::uint32_t victim,bool succeeds,bool reported)const {
        auto* a=w.population.findCitizen(author);
        auto* v=w.population.findCitizen(victim);
        if(!a||!v||a==v)throw std::invalid_argument("invalid crime participants");
        ++w.society.attempts;
        CrimeEvent e {
            author,victim,false,reported,false,false
        };
        Money amount=std::min(w.config.repairCost,w.ledger.balance(v->account));
        if(succeeds && amount>0)e.success=w.ledger.transfer(v->account,a->account,amount,TransactionType::Theft,w.clock.day());
        if(e.success) {
            ++w.society.actualCrimes;
            v->governmentTrust=std::max(0.f,v->governmentTrust-2);
        }
        if(reported) {
            ++w.society.reportedCrimes;
            if(e.success)e.restituted=w.ledger.transfer(a->account,v->account,amount,TransactionType::Restitution,w.clock.day());
            Money fine=std::min(w.ledger.balance(a->account),w.config.livingCost);
            if(fine>0)e.sanctioned=w.ledger.transfer(a->account,w.treasury.account(),fine,TransactionType::Fine,w.clock.day());
        }
        w.crimes.push_back(e);
        w.event(EventType::CrimeOccurred,author,e.success?"Theft succeeded; monetary transfer recorded":"Theft attempt failed");
        return e;
    }
    void CrimeSystem::update(World& w)const {
        const auto& people=w.population.getPopulation();
        if(people.size()<2)return;
        for(std::size_t i=0;i<people.size();++i) {
            const auto& p=people[i];
            double despair=(100-p.foodSatisfaction)/100.;
            double chance=w.config.crimeProbability*(.1+.5*despair+.4*p.riskTolerance)*(1-.5*w.government.policy.police);
            if(w.socialRandom.probability(chance)) {
                auto offset=w.socialRandom.randomInt(1,static_cast<int>(people.size()-1));
                auto& victim=people[(i+offset)%people.size()];
                bool success=w.socialRandom.probability(.7*(1-.5*w.government.policy.police));
                bool reported=w.socialRandom.probability(w.government.policy.police);
                attempt(w,p.getId(),victim.getId(),success,reported);
            }
        }
    }
}
