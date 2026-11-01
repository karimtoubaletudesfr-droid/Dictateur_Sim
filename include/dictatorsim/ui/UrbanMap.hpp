#pragma once
#include "UITheme.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
namespace ds {
class UrbanMap {
    sf::VertexArray city_{sf::Quads};
    std::size_t faceLimit_=400;
public:
    UrbanMap();
    void draw(sf::RenderTarget&,UITheme&,const PlayerSession&,bool expanded,sf::Vector2f mouse={-1,-1},bool click=false);
};
}
