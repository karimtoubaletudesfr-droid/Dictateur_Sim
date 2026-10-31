#include "TestWorld.hpp"
#include "dictatorsim/society/systems/ProtestSystem.hpp"
int main() {
    auto c=config();
    c.protestProbability=1;
    ds::World w(c);
    for(const auto& x:w.population.getPopulation()) {
        auto& p=*w.population.findCitizen(x.getId());
        p.anger=100;
        p.governmentTrust=0;
    }
    ds::ProtestSystem{}.update(w);
    assert(w.society.protesters>0);
    ds::World calm(c);
    for(const auto& x:calm.population.getPopulation()) {
        auto& p=*calm.population.findCitizen(x.getId());
        p.anger=0;
        p.governmentTrust=100;
    }
    ds::ProtestSystem{}.update(calm);
    assert(calm.society.protesters==0);
}
