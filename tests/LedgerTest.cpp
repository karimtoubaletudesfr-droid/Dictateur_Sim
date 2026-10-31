#include "dictatorsim/economy/Ledger.hpp"
#include "dictatorsim/core/RandomEngine.hpp"
#include <cassert>
#include <limits>
#include <stdexcept>
#include <iostream>
int main() {
    ds::Ledger a,b;
    for(auto* l:{&a,&b}) {
        l->openAccount(100000,"a");
        l->openAccount(100000,"b");
        l->seal();
    }
    assert(a.transfer(0,1,100,ds::TransactionType::Wage,1));
    assert(a.balance(0)==99900 && a.balance(1)==100100);
    for(auto n:{0,-1,200000}) assert(!a.transfer(0,1,n,ds::TransactionType::Purchase,1));
    assert(!a.transfer(99,1,1,ds::TransactionType::Tax,1));
    assert(!a.transfer(0,0,1,ds::TransactionType::Tax,1));
    assert(a.balance(0)==99900 && a.conserved());
    assert(a.transfer(1,0,100,ds::TransactionType::Restitution,1));
    ds::RandomEngine r1(42),r2(42);
    for(int i=0;i<100000;++i) {
        int f=r1.randomInt(0,1),g=r2.randomInt(0,1);
        int n=r1.randomInt(1,200),m=r2.randomInt(1,200);
        assert(a.transfer(f,1-f,n,ds::TransactionType::Purchase,i)==b.transfer(g,1-g,m,ds::TransactionType::Purchase,i));
        assert(a.conserved() && b.conserved());
    }
    assert(a.balance(0)==b.balance(0));
    ds::Ledger limit;
    limit.openAccount(std::numeric_limits<ds::Money>::max(),"max");
    bool threw=false;
    try {
        limit.openAccount(1,"overflow");
    }
    catch(const std::overflow_error&) {
        threw=true;
    }
    assert(threw);
    assert(ds::fraction(std::numeric_limits<ds::Money>::max(),10000)==std::numeric_limits<ds::Money>::max());
    std::cout<<"Ledger: 100000 reproducible transfers, conservation and refusals passed\n";
}
