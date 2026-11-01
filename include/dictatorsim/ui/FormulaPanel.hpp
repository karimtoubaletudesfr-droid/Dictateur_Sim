#pragma once
#include "UITheme.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
namespace ds {
class FormulaPanel {
    int category_=0;
public:
    void draw(sf::RenderTarget&,UITheme&,const PlayerSession&,sf::Vector2f,bool);
};
}
