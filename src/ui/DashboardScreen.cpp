#include "dictatorsim/ui/DashboardScreen.hpp"
namespace ds {
bool DashboardScreen::confirming()const{return decisions_.confirming();}
void DashboardScreen::scroll(sf::Vector2f mouse,float delta){if(mouse.x>=870)events_.scroll(delta);else if(mouse.x>=280)decisions_.scroll(delta);}
bool DashboardScreen::draw(sf::RenderTarget& t,UITheme& ui,PlayerSession& session,sf::Vector2f mouse,bool click,bool blocked){
    bool modal=blocked || session.request() || decisions_.confirming() || session.lost();bool normalClick=click&&!modal;
    ui.panel(t,"background",{0,0,1280,720},ui.background);
    ui.text(t,"brand","DICTATORSIM",24,20,28,ui.ink);ui.text(t,"subtitle","THE OFFICE OF THE PRESIDENT",26,56,11,ui.muted);
    ui.text(t,"date","DAY "+std::to_string(session.simulation().world().clock.day())+" / YEAR "+std::to_string(session.simulation().world().clock.year()),300,30,19,ui.ink);
    int speeds[]={0,1,2,5,10};const char* labels[]={"PAUSE","1x","2x","5x","10x"};
    for(int i=0;i<5;++i)if(ui.button(t,"speed"+std::to_string(i),labels[i],{550+i*88.f,24,78,40},mouse,normalClick,static_cast<int>(session.speed())==speeds[i],!modal))session.setSpeed(static_cast<SimulationSpeed>(speeds[i]));
    bool menu=ui.button(t,"menu","MENU / NEW GAME",{1040,24,220,40},mouse,normalClick);
    statistics_.draw(t,ui,session);events_.draw(t,ui,session);decisions_.draw(t,ui,session,mouse,normalClick);
    ui.text(t,"bottom","PLAYER GOVERNMENT  |  SPACE pause/resume  |  ESC menu  |  Mouse wheel: lists",24,681,13,ui.muted);
    ui.text(t,"accounting",session.simulation().report().observed.moneySupply==session.simulation().report().observed.initialMoney?"ACCOUNTS BALANCED":"ACCOUNTING ERROR",1030,681,13,ui.accent);
    if(session.request() && !blocked){
        const auto request=*session.request();
        ui.panel(t,"crisis-shade",{0,0,1280,720},{0,0,0,195});ui.panel(t,"crisis-panel",{300,150,680,430},ui.panelColor);
        ui.text(t,"crisis-title","URGENT BRIEFING / TIME PAUSED",328,178,22,ui.danger);
        ui.text(t,"crisis-desc",ui.wrap(request.prompt,68),328,226,16,ui.ink);
        for(std::size_t i=0;i<request.options.size();++i){
            if(ui.button(t,"crisis-option"+std::to_string(i),request.options[i].label,{328,320+float(i)*54,624,44},mouse,click)){
                if(!session.respond(i))crisisFeedback_="Action refused: insufficient funds or invalid conditions. Choose another response.";
                else crisisFeedback_.clear();
                break;
            }
        }
        ui.text(t,"crisis-feedback",ui.wrap(crisisFeedback_,76),328,546,12,ui.danger);
    }else if(decisions_.confirming() && !blocked)decisions_.drawConfirmation(t,ui,session,mouse,click);
    return menu;
}
}
