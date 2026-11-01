#include "dictatorsim/ui/DashboardScreen.hpp"
namespace ds {
bool DashboardScreen::confirming()const{return decisions_.confirming();}
void DashboardScreen::scroll(sf::Vector2f mouse,float delta){if(page_!=Page::Orders)return;if(mouse.x>=870)events_.scroll(delta);else if(mouse.x>=280)decisions_.scroll(delta);}
bool DashboardScreen::draw(sf::RenderTarget& t,UITheme& ui,PlayerSession& session,sf::Vector2f mouse,bool click,bool blocked){
    bool modal=blocked || session.request() || decisions_.confirming() || session.lost();bool normalClick=click&&!modal;
    if(page_==Page::Office){if(office_.draw(t,ui,session,mouse,normalClick))page_=Page::Orders;}
    else if(page_==Page::Map)map_.draw(t,ui,session,true,mouse,normalClick);
    else if(page_==Page::Formulas)formulas_.draw(t,ui,session,mouse,normalClick);
    else {ui.panel(t,"orders-bg",{0,0,1280,720},{23,40,41});statistics_.draw(t,ui,session);events_.draw(t,ui,session);decisions_.draw(t,ui,session,mouse,normalClick);}
    ui.panel(t,"header",{12,10,1256,72},{15,26,39,238});
    ui.text(t,"brand","DICTATORSIM",24,20,28,ui.ink);ui.text(t,"subtitle","LE CABINET / REPUBLIQUE DE DICTATORSIM",26,56,11,ui.muted);
    ui.text(t,"date","JOUR "+std::to_string(session.simulation().world().clock.day())+" / AN "+std::to_string(session.simulation().world().clock.year()),300,30,19,ui.ink);
    int speeds[]={0,1,2,5,10};const char* labels[]={"PAUSE","1x","2x","5x","10x"};
    for(int i=0;i<5;++i)if(ui.button(t,"speed"+std::to_string(i),labels[i],{550+i*88.f,24,78,40},mouse,normalClick,static_cast<int>(session.speed())==speeds[i],!modal))session.setSpeed(static_cast<SimulationSpeed>(speeds[i]));
    bool menu=ui.button(t,"menu","MENU",{1140,24,120,40},mouse,normalClick);
    if(ui.button(t,"map-toggle",page_==Page::Map?"BUREAU":"CARTE",{1020,24,110,40},mouse,normalClick))page_=page_==Page::Map?Page::Office:Page::Map;
    ui.panel(t,"footer",{12,672,1256,36},{15,26,39,238});
    const char* navigation[]={"01  BUREAU","02  CARTE","03  DOSSIERS","04  FORMULES"};
    for(int i=0;i<4;++i)if(ui.button(t,"nav-"+std::to_string(i),navigation[i],{24+i*165.f,677,153,29},mouse,normalClick,static_cast<int>(page_)==i))page_=static_cast<Page>(i);
    ui.text(t,"shortcuts","P : reglages  /  ESPACE : pause",712,685,11,ui.muted);
    ui.text(t,"accounting",session.simulation().report().observed.moneySupply==session.simulation().report().observed.initialMoney?"COMPTES EQUILIBRES":"ERREUR COMPTABLE",1030,681,13,ui.accent);
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
