#include "dictatorsim/ui/DecisionPanel.hpp"
#include "dictatorsim/politics/ActionValidator.hpp"
#include <algorithm>
namespace ds {
void DecisionPanel::scroll(float delta){if(history_)historyOffset_=std::max(0,historyOffset_+static_cast<int>(-delta));else offset_=std::clamp(offset_+static_cast<int>(-delta),0,static_cast<int>(PlayerDecisionController::catalog().size())-9);}
bool DecisionPanel::confirming()const{return confirmation_.has_value();}
void DecisionPanel::draw(sf::RenderTarget& t,UITheme& ui,PlayerSession& session,sf::Vector2f mouse,bool click){
    ui.panel(t,"decision-panel",{280,96,570,560},ui.panelColor);
    if(ui.button(t,"actions-tab","GOVERN",{298,110,140,38},mouse,click,!history_))history_=false;
    if(ui.button(t,"history-tab","DECISION HISTORY",{450,110,182,38},mouse,click,history_))history_=true;
    if(history_){
        const auto& records=session.decisions().history().records();
        historyOffset_=std::min(historyOffset_,std::max(0,static_cast<int>(records.size())-1));
        if(records.empty()){ui.text(t,"history-empty","No decisions yet. Your government is entirely manual.",298,180,14,ui.muted);return;}
        int idx=static_cast<int>(records.size())-1-historyOffset_;const auto& record=records[static_cast<std::size_t>(idx)];
        std::string label=record.action.type==ActionType::NoAction?"Acknowledged: no political action":"Action "+std::to_string(static_cast<int>(record.action.type));
        for(const auto& option:PlayerDecisionController::catalog())if(option.action.type==record.action.type)label=option.label;
        ui.text(t,"history-caption","DAY "+std::to_string(record.day)+" / "+(record.accepted?"ACCEPTED":"REFUSED")+" / scroll to browse",298,170,14,record.accepted?ui.accent:ui.danger);
        ui.text(t,"history-name",ui.wrap(label,50),298,202,18,ui.ink);
        ui.text(t,"history-parameters","Amount "+ui.money(record.action.amount)+" / bps "+std::to_string(record.action.basisPoints)+" / "+(record.action.visibility==ActionVisibility::Covert?"COVERT":"PUBLIC"),298,235,12,ui.muted);
        ui.text(t,"history-cost","Immediate public cost: "+ui.money(record.immediateCost),298,258,16,ui.ink);
        ui.text(t,"history-values","Treasury: "+ui.money(record.before.treasuryBalance)+" -> "+ui.money(record.after.treasuryBalance)+"\nStability: "+ui.number(record.before.nationalStability)+" -> "+ui.number(record.after.nationalStability)+"\nHappiness: "+ui.number(record.before.averageHappiness)+" -> "+ui.number(record.after.averageHappiness),298,298,16,ui.muted);
        ui.text(t,"history-result",ui.wrap(record.explanation.substr(0,180),58),298,386,14,ui.ink);
        std::string delayed="Day +30 observation pending.";
        if(record.afterThirtyDays){const auto& s=*record.afterThirtyDays;delayed="Day +30: treasury "+ui.money(s.treasuryBalance)+", happiness "+ui.number(s.averageHappiness)+", stability "+ui.number(s.nationalStability);}
        ui.text(t,"history-delayed",ui.wrap(delayed,55),298,458,15,ui.accent);
        ui.text(t,"history-caution","Observed changes have multiple causes.\nThey are not proof of a single decision's effect.",298,560,13,ui.muted);return;
    }
    const auto& options=PlayerDecisionController::catalog();
    for(int row=0;row<9;++row){int index=offset_+row;if(index>=static_cast<int>(options.size()))break;std::string label=options[index].label;if(label.size()>24)label=label.substr(0,21)+"...";
        if(ui.button(t,"action"+std::to_string(row),label,{298,164+row*46.f,208,40},mouse,click,index==selected_)){selected_=index;covert_=false;if(options[index].action.amount>0)allocation_=options[index].action.amount;}
    }
    ui.text(t,"action-scroll","Scroll here for more actions",298,592,12,ui.muted);
    const auto& selected=options[selected_];
    ui.text(t,"selected-title",ui.wrap(selected.label,26),524,168,18,ui.ink);
    ui.text(t,"selected-desc",ui.wrap(selected.description,33),524,244,14,ui.muted);
    PoliticalAction action=selected.action;
    bool spending=action.amount>0;
    if(spending){
        ui.text(t,"amount-label","Allocation: "+ui.money(allocation_),524,360,15,ui.ink);
        if(ui.button(t,"amount-less","- 1000",{524,392,112,36},mouse,click))allocation_=std::max<Money>(10000,allocation_-100000);
        if(ui.button(t,"amount-more","+ 1000",{650,392,112,36},mouse,click))allocation_=std::min<Money>(1000000000,allocation_+100000);
        action.amount=allocation_;
        if(action.type==ActionType::IncreaseMilitaryBudget)action.amount+=session.simulation().world().military.monthlyBudget;
    }
    if(action.type==ActionType::BuildHospital)action.amount=session.simulation().world().companies[3].price;
    bool mayHide=action.type==ActionType::ArrestOpponent || action.type==ActionType::PoliticalAssassination || action.type==ActionType::BanOrganization;
    if(mayHide && ui.button(t,"covert-toggle",covert_?"COVERT / exposure risk":"PUBLIC action",{524,440,280,36},mouse,click,covert_))covert_=!covert_;
    if(mayHide && covert_)action.visibility=ActionVisibility::Covert;
    auto refusal=ActionValidator{}.validate(session.simulation().world(),action);
    if(ui.button(t,"apply-action","REVIEW & APPLY",{524,490,280,42},mouse,click,false,!session.lost())){confirmation_=action;session.setSpeed(SimulationSpeed::Pause);}
    ui.text(t,"validity",ui.wrap(refusal.empty()?"Ready for validation.":"Currently unavailable: "+refusal,35),524,542,12,refusal.empty()?ui.accent:ui.danger);
    ui.text(t,"feedback",ui.wrap(feedback_,70),298,620,12,ui.muted);
}
void DecisionPanel::drawConfirmation(sf::RenderTarget& t,UITheme& ui,PlayerSession& session,sf::Vector2f mouse,bool click){
    if(!confirmation_)return;
    ui.panel(t,"confirm-shade",{0,0,1280,720},{0,0,0,185});ui.panel(t,"confirm-panel",{340,190,600,330},ui.panelColor);
    ui.text(t,"confirm-title","CONFIRM GOVERNMENT ORDER",366,214,22,ui.ink);
    std::string title;for(const auto& option:PlayerDecisionController::catalog())if(option.action.type==confirmation_->type)title=option.label+". "+option.description;
    ui.text(t,"confirm-description",ui.wrap(title,62),366,260,16,ui.muted);
    ui.text(t,"confirm-money","Amount / appropriation: "+ui.money(confirmation_->amount)+" | "+(confirmation_->visibility==ActionVisibility::Covert?"COVERT":"PUBLIC"),366,365,15,ui.ink);
    ui.text(t,"confirm-pause","Time remains paused after this order.",366,398,14,ui.muted);
    if(ui.button(t,"confirm-cancel","CANCEL",{366,452,190,42},mouse,click))confirmation_.reset();
    if(confirmation_ && ui.button(t,"confirm-yes","CONFIRM ORDER",{650,452,250,42},mouse,click)){auto result=session.decide(*confirmation_);feedback_=result.message;confirmation_.reset();}
}
}
