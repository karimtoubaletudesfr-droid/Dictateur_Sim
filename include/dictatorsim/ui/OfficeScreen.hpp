#pragma once
#include "UITheme.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
namespace ds {
class OfficeScreen {
    sf::Clock ambience_;
public:
    bool draw(sf::RenderTarget&,UITheme&,const PlayerSession&,sf::Vector2f,bool);
};
}
