#pragma once
#include "Event.hpp"
#include "EventSeverity.hpp"
#include "EventCategory.hpp"
namespace ds {
struct PlayerNotification {
    std::uint64_t id=0, day=0;
    EventSeverity severity=EventSeverity::Minor;
    EventCategory category=EventCategory::Economy;
    std::string message;
    bool requiresDecision=false, resolved=false;
};
class EventClassifier { public: static PlayerNotification classify(const Event& event); };
}
