#pragma once
#include "UITheme.hpp"
#include "StatisticsPanel.hpp"
#include "DecisionPanel.hpp"
#include "EventPanel.hpp"
namespace ds {
class DashboardScreen {
    StatisticsPanel statistics_;
    DecisionPanel decisions_;
    EventPanel events_;
    std::string crisisFeedback_;
public:
    bool draw(sf::RenderTarget& target,UITheme& theme,PlayerSession& session,sf::Vector2f mouse,bool click,bool blocked);
    void scroll(sf::Vector2f mouse,float delta);
    bool confirming() const;
};
}
