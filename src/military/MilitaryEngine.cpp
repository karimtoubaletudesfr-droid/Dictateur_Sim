#include "dictatorsim/military/MilitaryEngine.hpp"
#include "dictatorsim/military/systems/RecruitmentSystem.hpp"
#include "dictatorsim/military/systems/LoyaltySystem.hpp"
#include "dictatorsim/military/systems/CoupSystem.hpp"
namespace ds {
    void MilitaryEngine::update(World& w)const {
        RecruitmentSystem{}.update(w);
        LoyaltySystem{}.update(w);
        CoupSystem{}.update(w);
    }
}
