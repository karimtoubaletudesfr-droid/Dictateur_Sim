#pragma once
#include "EventType.hpp"
#include <cstdint>
#include <string>
namespace ds {
    struct Event {
        EventType type;
        std::uint64_t day;
        std::uint32_t subject;
        std::string message;
    };
}
