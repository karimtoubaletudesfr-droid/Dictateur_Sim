#include "TestWorld.hpp"
#include "dictatorsim/society/systems/RebellionSystem.hpp"
int main() {
    auto c=config();
    c.rebellionProbability=1;
    ds::World w(c);
    w.society.protesters=c.population;
    w.society.anger=100;
    w.government.state.legitimacy=0;
    ds::RebellionSystem{}.update(w);
    bool found=false;
    for(const auto& e:w.events.drain())if(e.type==ds::EventType::RebellionStarted)found=true;
    assert(found);
    ds::World calm(c);
    ds::RebellionSystem{}.update(calm);
    assert(!calm.society.rebellion);
}
