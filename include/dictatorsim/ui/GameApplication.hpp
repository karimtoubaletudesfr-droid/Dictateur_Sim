#pragma once
#include "UITheme.hpp"
#include "DashboardScreen.hpp"
#include "PauseMenu.hpp"
#include "SettingsPanel.hpp"
#include "AudioManager.hpp"
#include <memory>
namespace ds {
class GameApplication {
    SimulationConfig config_;
    AudioManager audio_;
    UITheme theme_;
    std::unique_ptr<PlayerSession> session_;
    DashboardScreen dashboard_;
    PauseMenu menu_;
    SettingsPanel settings_;
public:
    GameApplication(SimulationConfig config,const std::string& fontPath);
    int run(unsigned smokeFrames=0,unsigned width=1600,unsigned height=900,bool benchmark=false);
};
}
