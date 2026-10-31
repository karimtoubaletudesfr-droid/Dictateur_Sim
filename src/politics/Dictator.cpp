#include "dictatorsim/politics/Dictator.hpp"
#include <cmath>
#include <stdexcept>
namespace ds {
    Dictator::Dictator(Personality p):personality_(p) {
        for(double x:{p.ego,p.paranoia,p.greed,p.intelligence,p.empathy,p.authoritarianism,p.riskTolerance,p.ambition})if(!std::isfinite(x)||x<0||x>1)throw std::invalid_argument("invalid personality");
    }
    const Personality& Dictator::personality()const {
        return personality_;
    }
}
