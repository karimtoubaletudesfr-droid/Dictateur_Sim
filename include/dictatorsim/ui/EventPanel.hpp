#pragma once
#include "UITheme.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
namespace ds {
class EventPanel {
    int offset_=0;
public:
    void scroll(float delta);
    void draw(sf::RenderTarget& target,UITheme& theme,const PlayerSession& session);
};
}
