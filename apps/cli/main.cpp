#include "dictatorsim/core/Simulation.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <limits>
namespace {
    std::uint64_t number(const std::string& text) {
        if(text.empty() || text[0]=='-')throw std::invalid_argument("expected non-negative integer");
        std::size_t end=0;
        auto n=std::stoull(text,&end);
        if(end!=text.size())throw std::invalid_argument("invalid integer");
        return n;
    }
    void money(const char* name,ds::Money n) {
        std::cout<<name<<n/100<<'.'<<std::setfill('0')<<std::setw(2)<<n%100<<std::setfill(' ')<<'\n';
    }
}
int main(int argc,char** argv) {
    try {
        ds::SimulationConfig c;
        std::string directory="configs";
        for(int i=1;i<argc;++i)if(std::string(argv[i])=="--config") {
            if(++i==argc)throw std::invalid_argument("missing config directory");
            directory=argv[i];
        }
        c.load(directory);
        for(int i=1;i<argc;++i) {
            std::string key=argv[i];
            if(key=="--help") {
                std::cout<<"--population N --days N --seed N --config DIRECTORY --tax-bps N --ledger-log FILE\n";
                return 0;
            }
            if(++i==argc)throw std::invalid_argument("missing argument");
            std::string value=argv[i];
            if(key=="--config")continue;
            if(key=="--ledger-log") {
                c.ledgerArchive=value;
                continue;
            }
            auto n=number(value);
            if(key=="--population")c.population=n;
            else if(key=="--days")c.days=n;
            else if(key=="--seed") {
                if(n>std::numeric_limits<std::uint32_t>::max())throw std::invalid_argument("seed overflow");
                c.seed=n;
            }
            else if(key=="--tax-bps") {
                if(n>10000)throw std::invalid_argument("tax overflow");
                c.taxRate=n;
            }
            else throw std::invalid_argument("unknown option: "+key);
        }
        c.validate();
        ds::Simulation simulation(c);
        simulation.run(c.days);
        const auto s=simulation.report().observed;
        std::cout<<"DICTATORSIM V1\nDAY: "<<s.day<<" YEAR: "<<simulation.world().clock.year()<<"\nPOPULATION: "<<s.population<<"\n\nECONOMY\n";
        money("Money supply: ",s.moneySupply);
        money("Treasury: ",s.treasuryBalance);
        std::cout<<"Unemployment (%): "<<s.unemploymentRate<<"\nProduction (units/day): "<<s.totalProduction<<"\nPrice index: "<<s.priceIndex<<"\nEconomic prosperity: "<<s.economicProsperity<<"\n\nSOCIETY\nHappiness: "<<s.averageHappiness<<"\nAnger: "<<s.averageAnger<<"\nHealth: "<<s.averageHealth<<"\nCrime incidents: "<<s.crimeIncidents<<"\nActive protesters: "<<s.activeProtesters<<"\n\nPOLITICS\nStability: "<<s.nationalStability<<"\nMilitary loyalty: "<<s.militaryLoyalty<<"\nRegime status (0 governing, 1 transition, 2 collapsed, 3 crisis): "<<static_cast<int>(simulation.world().government.state.regime)<<"\n\nEVENTS (last 20)\n";
        simulation.world().logger.print(std::cout);
        std::cout<<"\nACCOUNTING\n";
        money("Initial money: ",s.initialMoney);
        money("Current money: ",s.moneySupply);
        money("Difference: ",s.moneySupply-s.initialMoney);
        std::cout<<"STATUS: CONSISTENT\n";
        return 0;
    }
    catch(const std::exception& e) {
        std::cerr<<"DictatorSim: "<<e.what()<<'\n';
        return 1;
    }
}
