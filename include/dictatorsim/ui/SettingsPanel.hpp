#pragma once
#include "UITheme.hpp"
#include "AudioManager.hpp"
#include "dictatorsim/core/PlayerSession.hpp"
#include "dictatorsim/core/RuntimeSettings.hpp"
namespace ds {
enum class SettingsChoice { None, Close, Restart };
class SettingsPanel {
    SimulationConfig draft_;
    int tab_=0, active_=-1, audioDrag_=-1;
    std::string input_, feedback_;
    bool confirmRestart_=false;
    bool commit();
public:
    void open(const PlayerSession& session);
    void event(const sf::Event& event);
    SettingsChoice draw(sf::RenderTarget&,UITheme&,PlayerSession&,AudioManager&,sf::Vector2f,bool,bool held=false);
    const SimulationConfig& draft() const { return draft_; }
};
}
