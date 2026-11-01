#pragma once
#include "UITheme.hpp"

namespace ds {
enum class MenuChoice { None, Return, Settings, NewGame, Quit };
class PauseMenu { public: MenuChoice draw(sf::RenderTarget& target,UITheme& theme,sf::Vector2f mouse,bool click,bool lost) const; };
}
