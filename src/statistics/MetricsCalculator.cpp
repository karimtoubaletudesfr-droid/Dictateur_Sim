#include "dictatorsim/statistics/MetricsCalculator.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
namespace ds {
    StatisticsSnapshot MetricsCalculator::calculate(const World& w)const {
        StatisticsSnapshot s;
        s.day=w.clock.day();
        s.population=w.population.getPopulationSize();
        std::size_t labor=0,unemployed=0;
        for(const auto& p:w.population.getPopulation())if(p.profession!=Profession::Student && p.profession!=Profession::Retired) {
            ++labor;
            if(p.employer<0 && p.profession!=Profession::Soldier)++unemployed;
        }
        s.unemploymentRate=labor?100.*unemployed/labor:0;
        s.averageHappiness=0;s.averageAnger=0;s.averageHealth=0;
        for(const auto& p:w.population.getPopulation()){s.averageHappiness+=p.happiness;s.averageAnger+=p.anger;s.averageHealth+=p.health;}
        if(s.population){s.averageHappiness/=s.population;s.averageAnger/=s.population;s.averageHealth/=s.population;}


        s.activeProtesters=w.society.protesters;
        s.crimeIncidents=w.society.actualCrimes;
        s.crimeRate=s.population && s.day?1000.*s.crimeIncidents/s.population/s.day:0;
        s.economicProsperity=s.population?std::clamp(50.*w.economy.purchases/s.population+50.*(1-s.unemploymentRate/100.),0.,100.):0;
        s.nationalStability=w.government.state.stability;
        s.militaryLoyalty=w.military.loyalty;
        s.nationalWellbeing=w.society.wellbeing;
        s.treasuryBalance=w.treasury.balance(w.ledger);
        s.moneySupply=w.ledger.moneySupply();
        s.initialMoney=w.ledger.initialSupply();
        s.totalProduction=w.economy.totalProduction;
        s.priceIndex=w.economy.priceIndex;
        return s;
    }
}
