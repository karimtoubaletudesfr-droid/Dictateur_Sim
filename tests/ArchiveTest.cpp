#include "dictatorsim/economy/Ledger.hpp"
#include <cassert>
#include <fstream>
#include <string>
int main() {
    ds::Ledger l;
    l.openAccount(100,"a");
    l.openAccount(100,"b");
    l.seal();
    l.archiveTo("build/archive-test.csv");
    for(int i=0;i<100005;++i)assert(l.transfer(i%2,1-i%2,1,ds::TransactionType::Purchase,i));
    l.flushArchive();
    assert(l.conserved());
    assert(l.transactions().size()==5);
    assert(l.transactions().back().id==100004);
    std::ifstream in("build/archive-test.csv");
    std::string row;
    unsigned count=0;
    while(std::getline(in,row))++count;
    assert(count==100006);
}
