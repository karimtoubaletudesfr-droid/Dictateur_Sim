#pragma once
#include <cstdint>
#include <vector>
namespace ds {
    struct SocialGroup {
        std::uint32_t region;
        std::vector<std::uint32_t> members;
    };
}
