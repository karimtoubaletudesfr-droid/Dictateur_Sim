#include "dictatorsim/economy/Account.hpp"
#include <cassert>
#include <stdexcept>
int main() {
    ds::Account a(7,123,"citizen");
    assert(a.id()==7 && a.balance()==123 && a.owner()=="citizen");
    bool bad=false;
    try {
        ds::Account b(0,-1,"bad");
    }
    catch(const std::invalid_argument&) {
        bad=true;
    }
    assert(bad);
}
