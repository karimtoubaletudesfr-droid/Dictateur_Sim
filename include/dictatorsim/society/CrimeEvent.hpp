#pragma once
#include <cstdint>
#include "CrimeType.hpp"
namespace ds {
    struct CrimeEvent {
        std::uint32_t author, victim;
        bool success, reported, restituted, sanctioned;
    };
}
