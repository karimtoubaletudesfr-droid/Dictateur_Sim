#pragma once
#include "Event.hpp"
#include <vector>
namespace ds {
    class EventBus {
        std::vector<Event> pending_;
        public:
        void publish(Event event);
        std::vector<Event> drain();
    };
}
