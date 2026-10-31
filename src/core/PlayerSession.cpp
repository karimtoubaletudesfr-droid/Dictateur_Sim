#include "dictatorsim/core/PlayerSession.hpp"
#include <cmath>
#include <stdexcept>
namespace ds {
namespace {SimulationConfig playerConfig(SimulationConfig c){c.playerMode=true;return c;}}
PlayerSession::PlayerSession(SimulationConfig c):simulation_(playerConfig(c)){}
void PlayerSession::setSpeed(SimulationSpeed s){
    if(s!=SimulationSpeed::Pause && s!=SimulationSpeed::Normal && s!=SimulationSpeed::Double && s!=SimulationSpeed::Fast && s!=SimulationSpeed::Fastest)throw std::invalid_argument("invalid speed");
    if((lost_ || request_) && s!=SimulationSpeed::Pause)return;
    speed_=s;
}
void PlayerSession::advance(double seconds){
    if(!std::isfinite(seconds)||seconds<0)throw std::invalid_argument("invalid elapsed time");
    if(speed_==SimulationSpeed::Pause || lost_ || request_)return;
    accumulator_+=seconds*static_cast<int>(speed_);
    // One simulated day per half-second at 1x; retain all backlog, max four days/frame.
    for(unsigned steps=0;steps<4 && accumulator_>=.5 && speed_!=SimulationSpeed::Pause;++steps){
        accumulator_-=.5;simulation_.step();decisions_.observe(simulation_.report().observed);collect();
    }
}
void PlayerSession::collect(){
    const auto& entries=simulation_.world().logger.entries();
    while(eventCursor_<entries.size())notify(EventClassifier::classify(entries[eventCursor_++]));
    auto report=simulation_.report().observed;
    if(report.population && report.activeProtesters>report.population/10)notify({0,report.day,EventSeverity::Major,EventCategory::Society,"Large demonstration: more than 10% of citizens mobilized",true,false});
    const auto& state=simulation_.world().government.state;
    if(state.regime==RegimeStatus::Collapsed || state.regime==RegimeStatus::Transition || state.leadershipChanges>0){lost_=true;speed_=SimulationSpeed::Pause;request_.reset();}
}
void PlayerSession::notify(PlayerNotification n){
    if(n.severity==EventSeverity::Minor)return; // Full minor journal remains in EventLogger.
    if(n.severity==EventSeverity::Critical)n.requiresDecision=true;
    auto found=cooldown_.find(n.category);
    if(n.requiresDecision && found!=cooldown_.end() && n.day<found->second && n.severity!=EventSeverity::Critical)n.requiresDecision=false;
    n.id=notifications_.size();notifications_.push_back(n);
    if(n.requiresDecision && !request_ && !lost_)activateNotification(n.id);
}
void PlayerSession::activateNotification(std::size_t index){
    const auto& n=notifications_.at(index);
    speed_=SimulationSpeed::Pause;cooldown_[n.category]=n.day+30;
    DecisionRequest request{n.id,simulation_.report(),n.message,{}};
    const auto& all=PlayerDecisionController::catalog();
    if(n.category==EventCategory::Disaster)request.options={all[7],all[3]};
    else if(n.category==EventCategory::Military)request.options={all[5],all[10]};
    else request.options={all[9],all[8],all[10]};
    request.options.push_back({"Take no action","Acknowledge the risk without changing policy.",{},false});
    request_=std::move(request);
}

DecisionResult PlayerSession::decide(PoliticalAction action){
    if(lost_)return {0,action,false,"You have lost power. Start a new game."};
    auto result=decisions_.submit(simulation_,action);collect();return result;
}
bool PlayerSession::respond(std::size_t option){
    if(!request_ || option>=request_->options.size() || lost_)return false;
    auto choice=request_->options[option];
    if(choice.hasAction){auto a=choice.action;if(a.type==ActionType::IncreaseMilitaryBudget)a.amount+=simulation_.world().military.monthlyBudget;if(!decide(a).approved)return false;}
    if(!choice.hasAction)decide({ActionType::NoAction});
    notifications_[request_->id].resolved=true;request_.reset();simulation_.acknowledgePlayerEvent();
    for(const auto& n:notifications_)if(n.requiresDecision && !n.resolved){activateNotification(n.id);break;}
    // No automatic resume. Even an explicit 'no action' leaves the game paused.
    return true;
}
SimulationSpeed PlayerSession::speed()const{return speed_;}
double PlayerSession::backlog()const{return accumulator_;}
bool PlayerSession::lost()const{return lost_;}
const Simulation& PlayerSession::simulation()const{return simulation_;}
const std::vector<PlayerNotification>& PlayerSession::notifications()const{return notifications_;}
const std::optional<DecisionRequest>& PlayerSession::request()const{return request_;}
const PlayerDecisionController& PlayerSession::decisions()const{return decisions_;}
}
