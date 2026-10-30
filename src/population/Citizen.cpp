#include "dictatorsim/population/Citizen.hpp"


namespace ds{
    // constructeur
    Citizen::Citizen(
        std::uint32_t id,
        std::uint8_t age,
        Profession profession
    )   
        :id(id),age(age),profession(profession),income(0.0),savings(0.0),foodSatisfaction(100.0f),
        health(100.0f),fatigue(0.0f),happiness(50.0f),governmentTrust(50.0f),anger(0.0f){}

    // geters

    std::uint32_t Citizen::getId()const{return this->id;}
    std::uint8_t Citizen::getAge()const{return this->age;}
    Profession Citizen::getProfession()const{return this->profession;}

    double Citizen::getIncome()const{return this->income;}
    double Citizen::getSavings()const{return this->savings;}

    float Citizen::getFoodSatisfaction() const{return this->foodSatisfaction;}
    float Citizen::getHealth()const{return this->health;}
    float Citizen::getFatigue()const{return this->fatigue;}

    float Citizen::getHappiness()const{return this->happiness;}
    float Citizen::getGovernmentTrust(){return this->governmentTrust;}
    float Citizen::getAnger()const{return this->anger;}
}