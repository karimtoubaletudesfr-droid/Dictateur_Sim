#include "dictatorsim/society/systems/HappinessSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void HappinessSystem::update(World& w) const {
        double happiness=0,anger=0,health=0;
        const auto& cfg=w.config;
        double weights=cfg.happinessFood+cfg.happinessHousing+cfg.happinessHealth+cfg.happinessRest;
        for(const auto& item:w.population.getPopulation()) {
            auto& p=*w.population.findCitizen(item.getId());
            bool working=p.employer>=0 || p.profession==Profession::Soldier;
            bool restDay=w.clock.day()%7==0;
            const auto& job=w.professionConfig(p.profession);
            float load=working&&!restDay?static_cast<float>(cfg.fatigue*job.physicalLoad*job.workHours/8):0;
            p.fatigue=std::clamp(p.fatigue+load-static_cast<float>(cfg.rest),0.f,100.f);
            p.stress=std::clamp(p.stress+(p.pendingCost>0?1.f:-.5f)+(working&&!restDay?static_cast<float>(.2*job.mentalLoad):-1.f),0.f,100.f);
            p.health=std::clamp(p.health+(p.foodSatisfaction<30?-.5f:.1f)+(p.pendingService==4&&p.pendingCost>0?-.2f:0.f),0.f,100.f);
            double target=(cfg.happinessFood*p.foodSatisfaction+cfg.happinessHousing*p.housing+cfg.happinessHealth*p.health+cfg.happinessRest*(100-p.fatigue))/weights-.15*p.stress;
            p.happiness=std::clamp(static_cast<float>(.9*p.happiness+.1*target),0.f,100.f);
            // Trust has its own inertia, injustice and policy exposure; anger is not 100-happiness.
            p.governmentTrust=std::clamp(static_cast<float>(p.governmentTrust+.01*(p.happiness-50)-.15*w.society.inequality-(w.government.policy.emergency?.1:0)-(w.government.policy.censorship?.05:0)-(w.government.policy.organizationBanned?.05:0)),0.f,100.f);
            p.anger=std::clamp(static_cast<float>(.95*p.anger+.05*(.45*(100-p.governmentTrust)+.3*(100-p.foodSatisfaction)+25*w.society.inequality)),0.f,100.f);
            happiness+=p.happiness;
            anger+=p.anger;
            health+=p.health;
        }
        auto n=w.population.getPopulationSize();
        w.society.happiness=n?happiness/n:0;
        w.society.anger=n?anger/n:0;
        w.society.health=n?health/n:0;
        w.society.wellbeing=.5*w.society.happiness+.5*w.society.health;
    }
}
