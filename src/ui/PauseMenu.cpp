#include "dictatorsim/ui/PauseMenu.hpp"
namespace ds {
MenuChoice PauseMenu::draw(sf::RenderTarget& t,UITheme& ui,sf::Vector2f mouse,bool click,bool lost)const{
    ui.panel(t,"menu-shade",{0,0,1280,720},{0,0,0,190});ui.panel(t,"menu-panel",{420,140,440,460},ui.panelColor);
    ui.text(t,"menu-title",lost?"YOU HAVE LOST POWER":"ADMINISTRATION PAUSED",446,202,23,lost?ui.danger:ui.ink);
    ui.text(t,"menu-desc",lost?"Your mandate has ended. Review or restart.":"Return to the country, or start again.",446,254,14,ui.muted);
    if(ui.button(t,"menu-return","RETURN TO DASHBOARD",{446,304,388,44},mouse,click,false,!lost))return MenuChoice::Return;
    if(ui.button(t,"menu-settings","PARAMETRES DU PAYS",{446,366,388,44},mouse,click,false,!lost))return MenuChoice::Settings;
    if(ui.button(t,"menu-new","NEW GAME (reset country)",{446,428,388,44},mouse,click))return MenuChoice::NewGame;
    if(ui.button(t,"menu-quit","QUIT",{446,490,388,44},mouse,click))return MenuChoice::Quit;
    return MenuChoice::None;
}
}
