#pragma once
#include "UITheme.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
namespace ds {
class DecisionPanel {
    int selected_=0, offset_=0, historyOffset_=0;
    bool history_=false, covert_=false;
    Money allocation_=1000000;
    std::optional<PoliticalAction> confirmation_;
    std::string feedback_="Select an action. The country starts paused.";
public:
    void scroll(float delta);
    void draw(sf::RenderTarget& target,UITheme& theme,PlayerSession& session,sf::Vector2f mouse,bool click);
    bool confirming() const;
    void drawConfirmation(sf::RenderTarget& target,UITheme& theme,PlayerSession& session,sf::Vector2f mouse,bool click);
};
}
