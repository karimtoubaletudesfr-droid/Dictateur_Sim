#pragma once
#include "UITheme.hpp"
#include "StatisticsPanel.hpp"
#include "DecisionPanel.hpp"
#include "EventPanel.hpp"
#include "UrbanMap.hpp"
#include "OfficeScreen.hpp"
#include "FormulaPanel.hpp"
namespace ds {
class DashboardScreen {
    UrbanMap map_;
    enum class Page { Office, Map, Orders, Formulas };
    Page page_=Page::Office;
    OfficeScreen office_;
    FormulaPanel formulas_;
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
