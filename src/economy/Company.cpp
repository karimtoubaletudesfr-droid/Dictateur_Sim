#include "dictatorsim/economy/Company.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <limits>
namespace ds {
    Company::Company(std::uint32_t i,Sector s,AccountId a,double p,Money price):id(i),sector(s),account(a),productivity(p),capacity(1),price(price) {
        if(!std::isfinite(p)||p<=0||price<=0)throw std::invalid_argument("invalid company");
    }
    double Company::produce(double labor,double multiplier) {
        if(!std::isfinite(labor)||!std::isfinite(multiplier)||labor<0||multiplier<0)throw std::invalid_argument("invalid production");
        double amount=labor*productivity*capacity*multiplier;
        // Storage capacity is 60 days of staffed output; excess production is not made.
        amount=std::min(amount,std::max(0.,labor*productivity*60-stock));
        stock+=amount;
        return amount;
    }
    bool Company::sell(Ledger& l,AccountId buyer,std::uint64_t day,TransactionType type) {
        if(stock<1)return false;
        if(revenue>std::numeric_limits<Money>::max()-price)throw std::overflow_error("revenue overflow");
        if(!l.transfer(buyer,account,price,type,day))return false;
        stock-=1;
        revenue+=price;
        return true;
    }
    void Company::damage(double f) {
        if(!std::isfinite(f)||f<0||f>1)throw std::invalid_argument("invalid damage");
        capacity*=1-f;
        stock*=1-f;
    }
}
