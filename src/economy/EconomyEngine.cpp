#include "dictatorsim/economy/systems/ProductionSystem.hpp"
#include "dictatorsim/economy/systems/WageSystem.hpp"
#include "dictatorsim/economy/systems/IncomeSystem.hpp"
#include "dictatorsim/economy/systems/ConsumptionSystem.hpp"
#include "dictatorsim/economy/systems/LivingCostSystem.hpp"
#include "dictatorsim/economy/systems/TaxationSystem.hpp"
#include "dictatorsim/economy/systems/TradeSystem.hpp"
#include "dictatorsim/economy/systems/PricingSystem.hpp"
#include "dictatorsim/economy/systems/InflationSystem.hpp"
#include "dictatorsim/economy/EconomyEngine.hpp"
namespace ds {
    void EconomyEngine::update(World& w)const {
        ProductionSystem{}.update(w);
        WageSystem{}.update(w);
        IncomeSystem{}.update(w);
        TaxationSystem{}.update(w);
        TradeSystem{}.update(w);
        ConsumptionSystem{}.update(w);
        LivingCostSystem{}.update(w);
        PricingSystem{}.update(w);
        InflationSystem{}.update(w);
    }
}
