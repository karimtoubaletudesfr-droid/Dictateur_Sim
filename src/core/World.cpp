#include "dictatorsim/core/World.hpp"
#include "dictatorsim/providers/CSVProfessionProvider.hpp"
#include <cmath>
#include <stdexcept>
namespace ds {
    World::World(SimulationConfig c):populationRandom_(c.seed),professions_(CSVProfessionProvider(c.configDirectory+"/professions.csv").loadWithEconomics(c.configDirectory+"/profession-economics.csv")),factory_(ids_,populationRandom_,professions_),config(c),population(factory_),treasury(0),economyRandom(c.seed^0x9e3779b9u),socialRandom(c.seed^0x85ebca6bu),militaryRandom(c.seed^0xc2b2ae35u),disasterRandom(c.seed^0x27d4eb2fu),politicalRandom(c.seed^0x165667b1u),government(c.taxRate,c.welfareRate,c.detection) {
        c.validate();
        for(auto& profession:professions_){
            auto i=static_cast<std::size_t>(profession.profession);
            if(config.salaries[i]>=0)profession.referenceSalary=config.salaries[i];
            else config.salaries[i]=profession.profession==Profession::Soldier?c.wage:profession.referenceSalary;
        }
        config.wage=std::max<Money>(1,config.salaries[static_cast<std::size_t>(Profession::Soldier)]);
        population.generatePopulation(c.population);
        Money citizens=c.population?fraction(c.moneySupply,c.citizenShare):0;
        Money firms=fraction(c.moneySupply,c.companyShare);
        treasury=Treasury(ledger.openAccount(c.moneySupply-citizens-firms,"government"));
        for(unsigned i=0;i<6;++i) {
            auto a=ledger.openAccount(firms/6+(i<static_cast<unsigned>(firms%6)),"company "+std::to_string(i));
            companies.emplace_back(i,static_cast<Sector>(i),a,c.productivity,i==0?c.foodPrice:c.livingCost);
        }
        for(const auto& original:population.getPopulation()) {
            auto& person=*population.findCitizen(original.getId());
            person.account=ledger.openAccount(citizens/static_cast<Money>(c.population)+(person.getId()<citizens%static_cast<Money>(c.population)),"citizen "+std::to_string(person.getId()));
            person.bindAccount(ledger,person.account);
            person.riskTolerance=populationRandom_.randomFloat(0,1);
            if(person.profession==Profession::Soldier)military.unit.citizens.push_back(person.getId());
            else if(person.profession!=Profession::Student && person.profession!=Profession::Retired && person.profession!=Profession::Unemployed) {
                unsigned sector=static_cast<unsigned>(professionConfig(person.profession).sector);
                if(person.profession==Profession::Worker)sector=person.getId()%2?1:2;
                person.employer=sector;
                companies[sector].employees.push_back(person.getId());
            }
        }
        military.monthlyBudget=fraction(treasury.balance(ledger),static_cast<int>(c.militaryShare*10000));
        ledger.seal();
        if(!c.ledgerArchive.empty())ledger.archiveTo(c.ledgerArchive);
        verify();
    }
    void World::event(EventType type,std::uint32_t id,const std::string& message) {
        events.publish({type,clock.day(),id,message});
    }
    void World::verify()const {
        if(!ledger.conserved())throw std::logic_error("money conservation violated");
        for(const auto& p:population.getPopulation()) {
            if(population.findCitizen(p.getId())!=&p)throw std::logic_error("population index mismatch");
            for(double x:{double(p.health),double(p.fatigue),double(p.happiness),double(p.anger),double(p.governmentTrust),double(p.foodSatisfaction),double(p.housing),double(p.stress)})if(!std::isfinite(x)||x<0||x>100)throw std::logic_error("invalid citizen indicator");
            if(p.pendingCost<0)throw std::logic_error("negative pending cost");
        }
        for(double x:{society.happiness,society.anger,society.health,society.wellbeing,military.loyalty,military.satisfaction,government.state.legitimacy,government.state.stability})if(!std::isfinite(x)||x<0||x>100)throw std::logic_error("invalid aggregate indicator");
        if(!std::isfinite(economy.priceIndex)||economy.priceIndex<=0||!std::isfinite(economy.totalProduction)||economy.totalProduction<0)throw std::logic_error("invalid economic indicator");
        for(const auto& c:companies)if(!std::isfinite(c.stock)||c.stock<0||!std::isfinite(c.capacity)||c.capacity<0||c.capacity>1)throw std::logic_error("invalid physical stock");
    }
}
namespace ds {
    const ProfessionConfig& World::professionConfig(Profession p)const {
        for(const auto& c:professions_)if(c.profession==p)return c;
        throw std::logic_error("unconfigured profession");
    }
}
