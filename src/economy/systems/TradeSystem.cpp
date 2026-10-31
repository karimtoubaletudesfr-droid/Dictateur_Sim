#include "dictatorsim/economy/systems/TradeSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
    void TradeSystem::update(World& w) const {
        // Domestic intermediate inputs: construction uses manufactured goods; factories use oil.
        for(int buyer:{2,3}) {
            auto& firm=w.companies[buyer];
            auto& supplier=w.companies[buyer==2?1:2];
            if(!firm.employees.empty() && supplier.sell(w.ledger,firm.account,w.clock.day(),TransactionType::Contract))firm.capacity=std::min(1.,firm.capacity+.001);
        }
    }
}
