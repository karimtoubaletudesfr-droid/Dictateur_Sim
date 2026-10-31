#pragma once
#include "UITheme.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
namespace ds {
class StatisticsPanel {
    sf::VertexArray graph_{sf::LineStrip};
    std::uint64_t graphDay_=~std::uint64_t(0);
public:
    void draw(sf::RenderTarget& target,UITheme& theme,const PlayerSession& session);
};
}
