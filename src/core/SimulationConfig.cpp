#include "dictatorsim/core/SimulationConfig.hpp"
#include <fstream>
#include <map>
#include <set>
#include <functional>
#include <cmath>
#include <stdexcept>
namespace ds {
    void SimulationConfig::load(const std::string& dir) {
        configDirectory=dir;
        std::map<std::string,std::function<void(const std::string&)>> fields;
        auto integer=[&](const std::string& key,auto& target) {
            fields[key]=[&target](const std::string& text) {
                std::size_t end=0;
                auto v=std::stoll(text,&end);
                if(end!=text.size() || v<0)throw std::runtime_error("invalid integer");
                target=static_cast<std::decay_t<decltype(target)>>(v);
                if(static_cast<unsigned long long>(target)!=static_cast<unsigned long long>(v))throw std::runtime_error("integer overflow");
            };
        };
        auto real=[&](const std::string& key,double& target) {
            fields[key]=[&target](const std::string& text) {
                std::size_t end=0;
                target=std::stod(text,&end);
                if(end!=text.size() || !std::isfinite(target))throw std::runtime_error("invalid real");
            };
        };
        integer("population",population);
        integer("seed",seed);
        integer("days",days);
        integer("money_supply",moneySupply);
        integer("citizen_share",citizenShare);
        integer("company_share",companyShare);
        integer("tax_rate",taxRate);
        integer("wage",wage);
        integer("food_price",foodPrice);
        integer("living_cost",livingCost);
        integer("repair_cost",repairCost);
        integer("medical_cost",medicalCost);
        integer("welfare_rate",welfareRate);
        real("productivity",productivity);
        real("fatigue",fatigue);
        real("rest",rest);
        real("crime_probability",crimeProbability);
        real("detection",detection);
        real("breakdown_probability",breakdownProbability);
        real("sickness_probability",sicknessProbability);
        real("disaster_probability",disasterProbability);
        real("happiness_food",happinessFood);
        real("happiness_housing",happinessHousing);
        real("happiness_health",happinessHealth);
        real("happiness_rest",happinessRest);
        real("protest_probability",protestProbability);
        real("rebellion_probability",rebellionProbability);
        real("coup_probability",coupProbability);
        real("military_share",militaryShare);
        real("corruption_probability",corruptionProbability);
        for(unsigned r=0;r<3;++r)for(unsigned h=0;h<5;++h)real("risk_"+std::to_string(r)+"_"+std::to_string(h),regionalRisks[r][h]);
        std::set<std::string> seen;
        for(const auto* name:{"simulation","demographics","economy","companies","resources","politics","military","society","disasters"}) {
            std::ifstream in(dir+"/"+name+".csv");
            if(!in)throw std::runtime_error("missing configuration: "+std::string(name));
            std::string line;
            while(std::getline(in,line)) {
                if(!line.empty() && line.back()=='\r')line.pop_back();
                if(line.empty() || line[0]=='#' || line=="key,value")continue;
                auto comma=line.find(',');
                if(comma==std::string::npos)throw std::runtime_error("invalid config row");
                auto key=line.substr(0,comma);
                auto it=fields.find(key);
                if(it==fields.end() || !seen.insert(key).second)throw std::runtime_error("unknown or repeated parameter: "+key);
                it->second(line.substr(comma+1));
            }
        }
        validate();
    }
    void SimulationConfig::validate() const {
        for(const auto& row:regionalRisks)for(double risk:row)if(!std::isfinite(risk)||risk<0||risk>1)throw std::invalid_argument("invalid regional risk");
        if(population>1000000 || days>1000000 || moneySupply<0 || citizenShare<0 || companyShare<0 || citizenShare>10000 || companyShare>10000 || citizenShare+companyShare>10000 || taxRate<0 || taxRate>10000 || welfareRate<0 || welfareRate>10000)throw std::invalid_argument("invalid simulation or allocation");
        for(double p:{crimeProbability,detection,breakdownProbability,sicknessProbability,disasterProbability,protestProbability,rebellionProbability,coupProbability,militaryShare,corruptionProbability})if(!std::isfinite(p)||p<0||p>1)throw std::invalid_argument("invalid probability");
        for(double v:{productivity,fatigue,rest,happinessFood,happinessHousing,happinessHealth,happinessRest})if(!std::isfinite(v)||v<0||v>100)throw std::invalid_argument("invalid coefficient");
        if(productivity==0 || happinessFood+happinessHousing+happinessHealth+happinessRest<=0)throw std::invalid_argument("zero coefficients");
        for(Money v:{wage,foodPrice,livingCost,repairCost,medicalCost})if(v<=0 || v>1000000000)throw std::invalid_argument("invalid cost");
    }
}
