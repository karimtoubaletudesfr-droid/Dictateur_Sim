#include "dictatorsim/ui/EventPanel.hpp"
#include <algorithm>
namespace ds {
void EventPanel::scroll(float delta){offset_=std::max(0,offset_+static_cast<int>(-delta*2));}
void EventPanel::draw(sf::RenderTarget& t,UITheme& ui,const PlayerSession& session){
    ui.panel(t,"event-panel",{870,96,390,560},ui.panelColor);ui.text(t,"event-title","COUNTRY DISPATCHES",888,112,17,ui.ink);
    ui.text(t,"event-hint","Recent facts / scroll to review",888,139,12,ui.muted);
    const auto& events=session.simulation().world().logger.entries();
    offset_=std::min(offset_,std::max(0,static_cast<int>(events.size())-1));
    for(int row=0;row<6;++row){int index=static_cast<int>(events.size())-1-offset_-row;if(index<0)break;
        const auto& e=events[static_cast<std::size_t>(index)];auto n=EventClassifier::classify(e);
        sf::Color color=n.severity>=EventSeverity::Major?ui.danger:n.severity==EventSeverity::Important?sf::Color(237,192,106):ui.muted;
        float y=174+row*75.f;
        const char* labels[]={"MINOR","IMPORTANT","MAJOR","CRITICAL"};
        ui.text(t,"event-meta"+std::to_string(row),"DAY "+std::to_string(e.day)+" / "+labels[static_cast<int>(n.severity)],888,y,12,color);
        auto message=e.message;if(message.size()>105)message=message.substr(0,102)+"...";
        ui.text(t,"event-text"+std::to_string(row),ui.wrap(message,43),888,y+19,13,ui.ink);
    }
    if(events.empty())ui.text(t,"event-empty","Your administration awaits your orders.",888,190,13,ui.muted);
}
}
