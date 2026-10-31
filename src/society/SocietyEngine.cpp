#include "dictatorsim/society/SocietyEngine.hpp"
#include "dictatorsim/society/systems/CrimeSystem.hpp"
#include "dictatorsim/society/systems/InequalitySystem.hpp"
#include "dictatorsim/society/systems/SocialInfluenceSystem.hpp"
#include "dictatorsim/society/systems/HappinessSystem.hpp"
#include "dictatorsim/society/systems/ProtestSystem.hpp"
#include "dictatorsim/society/systems/RebellionSystem.hpp"
namespace ds {
    void SocietyEngine::update(World& w)const {
        CrimeSystem{}.update(w);
        InequalitySystem{}.update(w);
        SocialInfluenceSystem{}.update(w);
        HappinessSystem{}.update(w);
        ProtestSystem{}.update(w);
        RebellionSystem{}.update(w);
    }
}
