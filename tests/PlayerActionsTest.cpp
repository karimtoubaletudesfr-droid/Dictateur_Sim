#include "TestWorld.hpp"
#include "dictatorsim/decisions/PlayerDecisionController.hpp"
#include "dictatorsim/politics/PolicyExecutor.hpp"
#include "dictatorsim/politics/ActionValidator.hpp"
int main(){
    for(const auto& option:ds::PlayerDecisionController::catalog()){
        ds::World w(config(100));auto action=option.action;
        w.companies[3].stock=10;w.government.policy.emergency=true;w.population.findCitizen(0)->protesting=true;w.society.protesters=1;
        if(action.type==ds::ActionType::ReleasePrisoners){w.government.state.prisoners=1;w.government.state.opponentImprisoned=true;}
        if(action.type==ds::ActionType::IncreaseMilitaryBudget)action.amount+=w.military.monthlyBudget;
        if(action.type==ds::ActionType::BuildHospital)action.amount=w.companies[3].price;
        if(action.type==ds::ActionType::DeclareEmergency)w.government.policy.emergency=false;
        if(action.type==ds::ActionType::ProvideDisasterRelief)w.population.findCitizen(0)->housing=30;
        auto treasury=w.treasury.balance(w.ledger);auto trust=w.population.findCitizen(0)->governmentTrust;
        auto productivity=w.companies[2].productivity;auto healthcare=w.companies[4].productivity;
        assert(ds::ActionValidator{}.validate(w,action).empty());assert(ds::PolicyExecutor{}.execute(w,action));w.verify();
        switch(action.type){
        case ds::ActionType::IncreaseTaxes:assert(w.government.policy.incomeTax==2000);break;
        case ds::ActionType::DecreaseTaxes:assert(w.government.policy.incomeTax==1000);break;
        case ds::ActionType::SubsidizeFood:assert(w.treasury.balance(w.ledger)==treasury-action.amount);break;
        case ds::ActionType::IncreaseHealthcareBudget:assert(w.companies[4].productivity>healthcare && w.companies[4].stock>0);break;
        case ds::ActionType::InvestInIndustry:assert(w.companies[2].productivity>productivity);break;
        case ds::ActionType::IncreaseMilitaryBudget:assert(w.military.monthlyBudget==action.amount && w.treasury.balance(w.ledger)==treasury);break;
        case ds::ActionType::DecreasePublicSpending:assert(w.government.policy.welfareRate==400);break;
        case ds::ActionType::ProvideDisasterRelief:assert(w.population.findCitizen(0)->pendingCost>0 && w.treasury.balance(w.ledger)<treasury);break;
        case ds::ActionType::AllowProtest:assert(w.government.policy.protestAllowed && w.population.findCitizen(0)->governmentTrust>trust);break;
        case ds::ActionType::NegotiateProtest:assert(w.society.protesters==0 && !w.population.findCitizen(0)->protesting);break;
        case ds::ActionType::DeclareEmergency:assert(w.government.policy.emergency);break;
        case ds::ActionType::EndEmergency:assert(!w.government.policy.emergency);break;
        case ds::ActionType::CensorMedia:assert(w.government.policy.censorship && w.government.state.fear>0);break;
        case ds::ActionType::BanOrganization:assert(w.government.policy.organizationBanned && w.government.state.radicalization>0);break;
        case ds::ActionType::ReleasePrisoners:assert(!w.government.state.prisoners && !w.government.state.opponentImprisoned);break;
        case ds::ActionType::ArrestOpponent:assert(w.government.state.opponentImprisoned && w.government.state.prisoners==1);break;
        case ds::ActionType::IncreasePoliceBudget:assert(w.government.policy.police>w.config.detection);break;
        case ds::ActionType::PoliticalAssassination:assert(!w.government.state.opponentAlive && w.government.state.victims==1);break;
        case ds::ActionType::ViolentRepression:case ds::ActionType::DestroyLocality:assert(w.population.findCitizen(0)->health<100 && w.government.state.victims>0);break;
        case ds::ActionType::IncreaseEducationBudget:assert(w.companies[2].productivity>productivity);break;
        case ds::ActionType::BuildHospital:assert(w.companies[3].stock==9 && w.companies[4].productivity>healthcare);break;
        default:assert(false);
        }
    }
    ds::World w(config(100));auto before=w.treasury.balance(w.ledger);double stock=w.companies[0].stock=100;double trust=w.population.findCitizen(0)->governmentTrust;
    assert(ds::PolicyExecutor{}.execute(w,{ds::ActionType::DestroyLocality,100000}));assert(w.companies[0].stock<stock);assert(w.population.findCitizen(0)->health<100);assert(w.population.findCitizen(0)->governmentTrust<trust);assert(w.government.state.victims>0);assert(w.treasury.balance(w.ledger)==before-100000);w.verify();
    ds::World x(config(50)),y(config(50));ds::PoliticalAction covert{ds::ActionType::ArrestOpponent,100000,0,ds::ActionVisibility::Covert};
    assert(ds::PolicyExecutor{}.execute(x,covert));assert(ds::PolicyExecutor{}.execute(y,covert));
    for(int i=0;i<1000;++i){x.clock.advance();y.clock.advance();ds::PoliticalConsequenceSystem{}.update(x);ds::PoliticalConsequenceSystem{}.update(y);}
    assert(x.government.state.scandals>0);assert(x.government.state.scandals==y.government.state.scandals);assert(x.population.findCitizen(0)->governmentTrust==y.population.findCitizen(0)->governmentTrust);x.verify();
}
