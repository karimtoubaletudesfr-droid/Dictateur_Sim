#pragma once
#include "EventBus.hpp"
#include <ostream>
namespace ds {
    class EventLogger {
        std::vector<Event> events_;
        public:
        void collect(EventBus& bus);
        const std::vector<Event>& entries() const;
        void print(std::ostream& out, std::size_t last=20) const;
    };
}
