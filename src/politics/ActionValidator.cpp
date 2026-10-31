#include "dictatorsim/politics/ActionValidator.hpp"
#include "dictatorsim/core/World.hpp"
namespace ds {
    std::string ActionValidator::validate(const World& w,const PoliticalAction& a)const {
        if(w.government.state.regime==RegimeStatus::Collapsed)return "government collapsed";
        if(a.amount<0 || a.basisPoints<0 || a.basisPoints>10000)return "invalid action values";
        if(a.visibility==ActionVisibility::Covert && a.type!=ActionType::ArrestOpponent && a.type!=ActionType::PoliticalAssassination && a.type!=ActionType::BanOrganization)return "this action cannot be covert";
        if(a.visibility!=ActionVisibility::Public && a.visibility!=ActionVisibility::Covert)return "invalid visibility";
        switch(a.type) {
            case ActionType::DecreasePublicSpending:
                return a.basisPoints>0 && a.basisPoints<=w.government.policy.welfareRate?"":"invalid welfare reduction";
            case ActionType::NoAction:return {};
            case ActionType::AllowProtest:return w.government.policy.protestAllowed?"assembly is already authorized":"";
            case ActionType::NegotiateProtest:return w.society.protesters?"":"no protesters to negotiate with";
            case ActionType::EndEmergency:return w.government.policy.emergency?"":"no emergency in force";
            case ActionType::ReleasePrisoners:return w.government.state.prisoners>0?"":"no political prisoners";
            case ActionType::CensorMedia:if(w.government.policy.censorship)return "censorship already active";break;
            case ActionType::BanOrganization:if(w.government.policy.organizationBanned)return "organization already banned";break;
            case ActionType::ArrestOpponent:if(!w.government.state.opponentAlive || w.government.state.opponentImprisoned)return "opponent unavailable";break;
            case ActionType::PoliticalAssassination:if(!w.government.state.opponentAlive)return "opponent already dead";break;
            case ActionType::ViolentRepression:if(w.society.protesters==0)return "no protesters";break;
            case ActionType::DestroyLocality:if(a.target>2 || w.population.getPopulationSize()==0)return "invalid locality";break;
            case ActionType::IncreaseTaxes:if(a.basisPoints>10000-w.government.policy.incomeTax)return "tax exceeds 100%";
            return {
            };
            case ActionType::DecreaseTaxes:if(a.basisPoints>w.government.policy.incomeTax)return "negative tax";
            return {
            };
            case ActionType::DeclareEmergency:return {
            };
            case ActionType::IncreaseMilitaryBudget:case ActionType::IncreaseEducationBudget:case ActionType::IncreaseHealthcareBudget:case ActionType::SubsidizeFood:case ActionType::BuildHospital:case ActionType::InvestInIndustry:case ActionType::IncreasePoliceBudget:case ActionType::ProvideDisasterRelief:break;
            default:return "unknown action";
        }
        if(static_cast<int>(a.type)>=static_cast<int>(ActionType::CensorMedia) && a.type!=ActionType::EndEmergency && a.amount<10000)return "minimum state operation cost is 10000 cents";
        if(a.type==ActionType::IncreaseMilitaryBudget && a.amount<=w.military.monthlyBudget)return "new appropriation must exceed current military budget";
        if(a.amount<=0 || a.amount>w.treasury.balance(w.ledger))return "insufficient public funds or zero appropriation";
        if((a.type==ActionType::SubsidizeFood || a.type==ActionType::ProvideDisasterRelief) && w.population.getPopulationSize()==0)return "no beneficiaries";
        if(a.type==ActionType::BuildHospital && w.companies[3].stock<1)return "construction materials unavailable";
        if(a.type==ActionType::BuildHospital && a.amount!=w.companies[3].price)return "hospital contract must match construction quote";
        return {
        };
    }
}
