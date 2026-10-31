#include "TestWorld.hpp"
#include "dictatorsim/society/systems/HappinessSystem.hpp"
int main() {
    ds::World fed(config(1)),hungry(config(1));
    auto& p=*hungry.population.findCitizen(0);
    p.foodSatisfaction=0;
    p.housing=0;
    for(int i=0;i<20;++i) {
        ds::HappinessSystem{}.update(fed);
        ds::HappinessSystem{}.update(hungry);
    }
    assert(fed.society.happiness>hungry.society.happiness);
    assert(p.anger!=100-p.happiness);
    p.fatigue=50;
    p.profession=ds::Profession::Unemployed;
    p.employer=-1;
    ds::HappinessSystem{}.update(hungry);
    assert(p.fatigue<50);
    hungry.verify();
}
