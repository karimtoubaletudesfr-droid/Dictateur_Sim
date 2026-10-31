#include "dictatorsim/ui/StatisticsPanel.hpp"
#include <array>
namespace ds {
void StatisticsPanel::draw(sf::RenderTarget& t,UITheme& ui,const PlayerSession& session){
    const auto s=session.simulation().report().observed;
    const auto& state=session.simulation().world().government.state;
    ui.panel(t,"stats-panel",{20,96,240,560},ui.panelColor);
    ui.text(t,"country-label","STATE OF THE NATION",36,112,16,ui.muted);
    std::array<std::pair<std::string,std::string>,12> rows{{
        {"Population",std::to_string(s.population)},{"Treasury",ui.money(s.treasuryBalance)},
        {"Money supply",ui.money(s.moneySupply)},{"Prosperity",ui.number(s.economicProsperity)},
        {"Happiness",ui.number(s.averageHappiness)},{"Anger",ui.number(s.averageAnger)},
        {"Stability",ui.number(s.nationalStability)},{"Army loyalty",ui.number(s.militaryLoyalty)},
        {"Unemployment",ui.number(s.unemploymentRate)+"%"},{"Thefts (total)",std::to_string(s.crimeIncidents)},
        {"Fear / radicalization",ui.number(state.fear)+" / "+ui.number(state.radicalization)},
        {"Victims / prisoners",std::to_string(state.victims)+" / "+std::to_string(state.prisoners)}
    }};
    for(std::size_t i=0;i<rows.size();++i){float y=150+float(i)*32;ui.text(t,"metric-name"+std::to_string(i),rows[i].first,36,y,12,ui.muted);ui.text(t,"metric-value"+std::to_string(i),rows[i].second,36,y+13,15,ui.ink);}
    ui.text(t,"graph-label","STABILITY - last 120 observations",36,548,11,ui.muted);
    if(graphDay_!=s.day){
        graphDay_=s.day;const auto& history=session.simulation().statistics().history().snapshots();
        std::size_t start=history.size()>120?history.size()-120:0;graph_.resize(history.size()-start);
        for(std::size_t i=start;i<history.size();++i){graph_[i-start].position={36+208.f*float(i-start)/119.f,636-static_cast<float>(history[i].nationalStability)*.6f};graph_[i-start].color=ui.accent;}
    }
    t.draw(graph_);
}
}
