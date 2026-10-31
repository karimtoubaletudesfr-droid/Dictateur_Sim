#include "TestWorld.hpp"
int main() {
    ds::Ledger l;
    auto a=l.openAccount(100,"buyer"),b=l.openAccount(0,"seller");
    l.seal();
    ds::Company c(0,ds::Sector::Agriculture,b,2,50);
    assert(!c.sell(l,a,1));
    assert(c.produce(2,1)==4);
    assert(c.sell(l,a,1));
    assert(c.stock==3 && l.balance(a)==50 && l.balance(b)==50);
    c.damage(.5);
    assert(c.stock==1.5 && c.capacity==.5 && l.conserved());
}
