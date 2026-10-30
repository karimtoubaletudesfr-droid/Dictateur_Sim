#include "dictatorsim/population/Citizen.hpp"

#include <cassert>
#include <iostream>

using namespace ds;

int main() {
    Citizen citizen(1, 25, Profession::Engineer);

    assert(citizen.getId() == 1);
    assert(citizen.getAge() == 25);
    assert(citizen.getProfession() == Profession::Engineer);

    assert(citizen.getIncome() == 0.0);
    assert(citizen.getSavings() == 0.0);

    assert(citizen.getFoodSatisfaction() == 100.0f);
    assert(citizen.getHealth() == 100.0f);
    assert(citizen.getFatigue() == 0.0f);

    assert(citizen.getHappiness() == 50.0f);
    assert(citizen.getGovernmentTrust() == 50.0f);
    assert(citizen.getAnger() == 0.0f);

    std::cout << "All Citizen tests passed!\n";

    return 0;
}