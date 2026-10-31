#include "TestWorld.hpp"
#include "dictatorsim/core/SimulationClock.hpp"
int main() {
    ds::SimulationClock c;
    assert(c.day()==0 && !c.monthEnd());
    for(int i=0;i<30;++i)c.advance();
    assert(c.monthEnd() && c.month()==2);
    for(int i=30;i<360;++i)c.advance();
    assert(c.year()==2 && c.month()==1);
}
