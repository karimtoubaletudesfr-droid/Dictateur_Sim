#include "dictatorsim/core/SimulationClock.hpp"
#include <limits>
#include <stdexcept>
namespace ds {
    void SimulationClock::advance() {
        if(elapsed_==std::numeric_limits<std::uint64_t>::max())throw std::overflow_error("clock overflow");
        ++elapsed_;
    }
    std::uint64_t SimulationClock::day()const {
        return elapsed_;
    }
    unsigned SimulationClock::month()const {
        return static_cast<unsigned>((elapsed_/30)%12)+1;
    }
    std::uint64_t SimulationClock::year()const {
        return elapsed_/360+1;
    }
    bool SimulationClock::monthEnd()const {
        return elapsed_>0 && elapsed_%30==0;
    }
}
