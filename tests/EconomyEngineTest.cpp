#include "TestWorld.hpp"
#include "dictatorsim/economy/EconomyEngine.hpp"
int main() {
    ds::World w(config());
    for(int i=0;i<360;++i) {
        w.clock.advance();
        ds::EconomyEngine{}.update(w);
        w.verify();
    }
    assert(!w.ledger.transactions().empty());
    assert(w.economy.totalProduction>=0);
    ds::World empty(config(0));
    empty.clock.advance();
    ds::EconomyEngine{}.update(empty);
    empty.verify();
}
