#pragma once

#include <cstdint>
#include "dictatorsim/economy/Money.hpp"
#include "Profession.hpp"
namespace ds {
class Ledger;

struct Citizen {
    // Identité
private:
    std::uint32_t id;
    const Ledger* ledger_ = nullptr;
public:
    std::uint8_t age;
    Profession profession;
    // Situation économique
    double income;
    void bindAccount(const Ledger& ledger, AccountId id);
    AccountId account = 0;
    int employer = -1;
    Money earned = 0;
    Money pendingCost = 0;
    int pendingService = -1;
    float housing = 100, stress = 0, riskTolerance = 0.5f;
    bool protesting = false;
    // Besoins
    float foodSatisfaction;
    float health;
    float fatigue;

    // État psychologique et politique
    float happiness;
    float governmentTrust;
    float anger;

    public:
    Citizen(
        std::uint32_t id,
        std::uint8_t age,
        Profession profession
        );
    std::uint32_t getId()const;
    std::uint8_t getAge()const;
    Profession getProfession()const;

    double getIncome()const;
    double getSavings()const;


    float getFoodSatisfaction()const;
    float getHealth()const;
    float getFatigue()const;


    float getHappiness()const;
    float getGovernmentTrust() const;
    float getAnger()const;


};

}