#include "dictatorsim/economy/Market.hpp"
namespace ds {
    Company* Market::seller(std::vector<Company>& cs,Sector s) {
        for(auto& c:cs)if(c.sector==s)return &c;
        return nullptr;
    }
}
