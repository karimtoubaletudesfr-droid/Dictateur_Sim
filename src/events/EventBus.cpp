#include "dictatorsim/events/EventBus.hpp"
#include <utility>
namespace ds {
    void EventBus::publish(Event e) {
        pending_.push_back(std::move(e));
    }
    std::vector<Event> EventBus::drain() {
        std::vector<Event> result;
        result.swap(pending_);
        return result;
    }
}
