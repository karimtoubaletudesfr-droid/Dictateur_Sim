#pragma once
#include <cstdint>
namespace ds {
    class SimulationClock {
        std::uint64_t elapsed_=0;
        public:
        void advance();
        std::uint64_t day() const;
        unsigned month() const;
        std::uint64_t year() const;
        bool monthEnd() const;
    };
}
