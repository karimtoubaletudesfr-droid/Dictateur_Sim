#pragma once
#include <cstddef>
namespace ds {
    struct SocialState {
        double happiness=50, anger=0, health=100, inequality=0, wellbeing=50;
        std::size_t protesters=0, actualCrimes=0, reportedCrimes=0, attempts=0;
        bool rebellion=false;
    };
}
