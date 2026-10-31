#include "dictatorsim/decisions/PlayerDecisionController.hpp"
#include "dictatorsim/core/Simulation.hpp"
#include "dictatorsim/politics/ActionValidator.hpp"
namespace ds {
DecisionResult PlayerDecisionController::submit(Simulation& simulation,const PoliticalAction& action) {
    auto before=simulation.report().observed;
    auto taxBefore=simulation.world().government.policy.incomeTax;
    auto victimsBefore=simulation.world().government.state.victims;
    auto prisonersBefore=simulation.world().government.state.prisoners;
    auto capacityBefore=simulation.world().companies[4].productivity;
    auto reason=ActionValidator{}.validate(simulation.world(),action);
    bool ok=reason.empty() && simulation.act(action);
    auto after=simulation.report().observed;
    if(ok){
        reason="Applied. ";
        for(const auto& option:catalog())if(option.action.type==action.type)reason+=option.description+" ";
        reason+="Tax bps: "+std::to_string(taxBefore)+" -> "+std::to_string(simulation.world().government.policy.incomeTax);
        if(simulation.world().government.state.victims!=victimsBefore)reason+="; victim events +"+std::to_string(simulation.world().government.state.victims-victimsBefore);
        if(simulation.world().government.state.prisoners!=prisonersBefore)reason+="; prisoners "+std::to_string(simulation.world().government.state.prisoners);
        if(simulation.world().companies[4].productivity!=capacityBefore)reason+="; healthcare productivity "+std::to_string(simulation.world().companies[4].productivity);
    }
    else if(reason.empty())reason="Payment or execution refused.";
    history_.add({before.day,action,ok,before.treasuryBalance-after.treasuryBalance,reason,before,after,{}});
    return {history_.records().size()-1,action,ok,reason};
}
void PlayerDecisionController::observe(const StatisticsSnapshot& s){history_.observe(s);}
const DecisionHistory& PlayerDecisionController::history()const{return history_;}
const std::vector<DecisionOption>& PlayerDecisionController::catalog(){
    static const std::vector<DecisionOption> options={
        {"Raise income tax +5 points","Next payroll: more tax, less disposable income.",{ActionType::IncreaseTaxes,0,500}},
        {"Lower income tax -5 points","Next payroll: less revenue, more disposable income.",{ActionType::DecreaseTaxes,0,500}},
        {"Food support","Transfer 10000 units to households; consumption uses their balances.",{ActionType::SubsidizeFood,1000000}},
        {"Fund hospitals","Pay healthcare provider: treatment stock and productivity increase.",{ActionType::IncreaseHealthcareBudget,1000000}},
        {"Invest in industry","Pay industry: productive capacity and output potential improve.",{ActionType::InvestInIndustry,1000000}},
        {"Raise military appropriation","Increase the monthly limit by 10000 units; actual wages remain funded.",{ActionType::IncreaseMilitaryBudget,1000000}},
        {"Reduce public welfare","Reduce monthly welfare allocation by 1 percentage point.",{ActionType::DecreasePublicSpending,0,100}},
        {"Disaster reconstruction","Fund households; damaged homes receive persistent repair needs.",{ActionType::ProvideDisasterRelief,1000000}},
        {"Allow demonstration","Recognize assembly: direct trust response; no automatic stability bonus.",{ActionType::AllowProtest}},
        {"Negotiate with protesters","Dialogue lowers anger and ends current participation; grievances may return.",{ActionType::NegotiateProtest}},
        {"Declare emergency","Limits protest participation, but trust erodes daily.",{ActionType::DeclareEmergency}},
        {"End emergency","End extraordinary restrictions.",{ActionType::EndEmergency}},
        {"Censor fictional media","Cost 1000 units. Fear and outrage; recurring loss of trust.",{ActionType::CensorMedia,100000}},
        {"Ban Civic Assembly","Cost 1000 units. Fictional organization; radicalization and trust loss.",{ActionType::BanOrganization,100000}},
        {"Release political prisoners","Release all detainees; fear falls and trust improves.",{ActionType::ReleasePrisoners}},
        {"Imprison opponent Mara Voss","Cost 1000 units. Arbitrary detention; fear, outrage and military dissent.",{ActionType::ArrestOpponent,100000}},
        {"Strengthen security services","Pay 10000 units to services; detection probability rises.",{ActionType::IncreasePoliceBudget,1000000}},
        {"Assassinate Mara Voss [fiction]","ABSTRACT VIOLENCE: one victim, serious outrage and military dissent.",{ActionType::PoliticalAssassination,100000}},
        {"Violently repress protest [fiction]","ABSTRACT VIOLENCE: injured protesters, detentions and radicalization.",{ActionType::ViolentRepression,100000}},
        {"Destroy locality Northbank [fiction]","ABSTRACT VIOLENCE: region 0 assets and housing destroyed; citizens injured.",{ActionType::DestroyLocality,100000}},
        {"Fund education","Pay 10000 units to services; productive skills improve gradually.",{ActionType::IncreaseEducationBudget,1000000}},
        {"Build hospital","Pay exact construction quote and consume one construction unit.",{ActionType::BuildHospital}}
    };
    return options;
}
}
