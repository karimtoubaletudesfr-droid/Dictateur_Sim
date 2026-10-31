#include "dictatorsim/society/systems/InequalitySystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void InequalitySystem::update(World& w) const {
        std::vector<Money> balances;
        long double sum=0,weighted=0;
        for(const auto& p:w.population.getPopulation()) {
            auto b=w.ledger.balance(p.account);
            balances.push_back(b);
            sum+=b;
        }
        std::sort(balances.begin(),balances.end());
        for(std::size_t i=0;i<balances.size();++i)weighted+=(2*static_cast<long double>(i)+1)*balances[i];
        w.society.inequality=sum>0?static_cast<double>(weighted/(balances.size()*sum)-1):0;
    }
}
