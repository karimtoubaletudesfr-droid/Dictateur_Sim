#pragma once
#include <cstddef>
#include <array>
#include <cstdint>
#include <string>
#include "dictatorsim/economy/Money.hpp"
namespace ds {
    struct SimulationConfig {
        bool playerMode=false;
        std::array<Money,11> salaries{{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
        double rebellionParticipation=0, coupLoyaltyThreshold=100;
        std::size_t population=1000;
        std::uint32_t seed=42;
        std::uint64_t days=365;
        Money moneySupply=1000000000;
        int citizenShare=4000, companyShare=4000, taxRate=1500;
        Money wage=60000, foodPrice=150, livingCost=300, repairCost=15000, medicalCost=10000;
        std::array<std::array<double,5>,3> regionalRisks {
            {
                {
                    {
                        .6,1,.4,1,0
                    }
                }
                , {
                    {
                        .3,0,1,.6,0
                    }
                }
                , {
                    {
                        1,0,.3,.2,.2
                    }
                }
            }
        };
        double productivity=30, fatigue=2, rest=1.8, crimeProbability=.002, detection=.4;
        double breakdownProbability=.0005, sicknessProbability=.0005, disasterProbability=.001;
        double happinessFood=.35, happinessHousing=.2, happinessHealth=.25, happinessRest=.2;
        double protestProbability=.08, rebellionProbability=.02, coupProbability=.02;
        double militaryShare=.1, corruptionProbability=.001;
        int welfareRate=500;
        std::string configDirectory="configs";
        std::string ledgerArchive;
        void load(const std::string& directory);
        void validate() const;
    };
}
