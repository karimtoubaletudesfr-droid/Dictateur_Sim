#include "dictatorsim/politics/PolicyExecutor.hpp"
#include "dictatorsim/politics/ActionValidator.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
namespace ds {
    bool PolicyExecutor::execute(World& w,const PoliticalAction& a)const {
        auto error=ActionValidator{}.validate(w,a);
        if(!error.empty()) {
            w.event(EventType::PaymentRefused,0,"Policy refused: "+error);
            return false;
        }
        auto& state=w.government.state;
        auto publicReaction=[&](double trust,double anger) {
            for(const auto& item:w.population.getPopulation()) {
                auto& p=*w.population.findCitizen(item.getId());
                p.governmentTrust=std::clamp(p.governmentTrust+static_cast<float>(trust),0.f,100.f);
                p.anger=std::clamp(p.anger+static_cast<float>(anger),0.f,100.f);
            }
        };
        switch(a.type) {
            case ActionType::NoAction:break;
            case ActionType::DecreasePublicSpending:w.government.policy.welfareRate-=a.basisPoints;break;
            case ActionType::EndEmergency:w.government.policy.emergency=false;break;
            case ActionType::AllowProtest:w.government.policy.protestAllowed=true;publicReaction(2,-2);break;
            case ActionType::NegotiateProtest:
                publicReaction(3,-5);
                for(const auto& item:w.population.getPopulation())if(item.protesting)w.population.findCitizen(item.getId())->protesting=false;
                w.society.protesters=0;break;
            case ActionType::ReleasePrisoners:
                state.prisoners=0;state.opponentImprisoned=false;state.fear=std::max(0.,state.fear-5);publicReaction(5,-3);break;
            case ActionType::CensorMedia:case ActionType::BanOrganization:case ActionType::ArrestOpponent:
            case ActionType::PoliticalAssassination:case ActionType::ViolentRepression:case ActionType::DestroyLocality: {
                if(!w.treasury.pay(w.ledger,w.companies[5].account,a.amount,TransactionType::Contract,w.clock.day()))return false;
                double severity=5;
                if(a.type==ActionType::CensorMedia)w.government.policy.censorship=true;
                if(a.type==ActionType::BanOrganization)w.government.policy.organizationBanned=true;
                if(a.type==ActionType::ArrestOpponent){state.opponentImprisoned=true;++state.prisoners;severity=10;}
                if(a.type==ActionType::PoliticalAssassination){state.opponentAlive=false;if(state.opponentImprisoned){state.opponentImprisoned=false;--state.prisoners;}++state.victims;severity=25;}
                if(a.type==ActionType::ViolentRepression || a.type==ActionType::DestroyLocality) {
                    severity=a.type==ActionType::DestroyLocality?35:20;
                    for(const auto& item:w.population.getPopulation())if(a.type==ActionType::DestroyLocality?item.getId()%3==a.target:item.protesting) {
                        auto& p=*w.population.findCitizen(item.getId());
                        p.health=std::max(0.f,p.health-20);p.stress=std::min(100.f,p.stress+30);++state.victims;
                        if(a.type==ActionType::DestroyLocality)p.housing=std::max(0.f,p.housing-60);
                        p.protesting=false;
                    }
                    if(a.type==ActionType::DestroyLocality)for(auto& firm:w.companies)if(firm.id%3==a.target)firm.damage(.5);
                    state.prisoners+=a.type==ActionType::ViolentRepression?static_cast<unsigned>(w.society.protesters/4):0;
                    w.society.protesters=0;for(const auto& p:w.population.getPopulation())if(p.protesting)++w.society.protesters;
                }
                state.fear=std::min(100.,state.fear+severity);
                w.military.loyalty=std::max(0.,w.military.loyalty-severity*.25);
                if(a.visibility==ActionVisibility::Covert)w.politicalConsequences.push_back({w.clock.day(),severity,false});
                else {publicReaction(-severity,severity*.5);state.radicalization=std::min(100.,state.radicalization+severity);}
                break;
            }
            case ActionType::IncreaseTaxes:w.government.policy.incomeTax+=a.basisPoints;
            break;
            case ActionType::DecreaseTaxes:w.government.policy.incomeTax-=a.basisPoints;
            break;
            case ActionType::DeclareEmergency:w.government.policy.emergency=true;
            break;
            case ActionType::IncreaseMilitaryBudget:w.military.monthlyBudget=a.amount;
            break;
            case ActionType::SubsidizeFood:case ActionType::ProvideDisasterRelief: {
                auto n=static_cast<Money>(w.population.getPopulationSize());
                Money i=0;
                for(const auto& p:w.population.getPopulation()) {
                    Money share=a.amount/n+(i++<a.amount%n);
                    if(share>0)w.treasury.pay(w.ledger,p.account,share,TransactionType::Subsidy,w.clock.day());
                }
                if(a.type==ActionType::ProvideDisasterRelief)for(const auto& item:w.population.getPopulation()) {
                    auto& p=*w.population.findCitizen(item.getId());
                    if(p.housing<100 && p.pendingCost==0){p.pendingCost=w.config.repairCost;p.pendingService=3;}
                }
                break;
            }
            default: {
                int sector=a.type==ActionType::BuildHospital?3:a.type==ActionType::IncreaseHealthcareBudget?4:a.type==ActionType::InvestInIndustry?2:5;
                if(!w.treasury.pay(w.ledger,w.companies[sector].account,a.amount,TransactionType::Contract,w.clock.day()))return false;
                if(a.type==ActionType::BuildHospital) {
                    w.companies[3].stock-=1;
                    w.companies[4].capacity=std::min(1.,w.companies[4].capacity+.1);
                    w.companies[4].productivity+=.1;
                }
                if(a.type==ActionType::InvestInIndustry){double units=static_cast<double>(a.amount)/w.companies[2].price;w.companies[2].capacity=std::min(1.,w.companies[2].capacity+units*.001);w.companies[2].productivity+=units*.001;}
                if(a.type==ActionType::IncreasePoliceBudget)w.government.policy.police=std::min(1.,w.government.policy.police+.05);
                if(a.type==ActionType::IncreaseEducationBudget)for(auto& firm:w.companies)firm.productivity+=.01;
                if(a.type==ActionType::IncreaseHealthcareBudget){w.companies[4].productivity+=static_cast<double>(a.amount)/w.companies[4].price*.001;w.companies[4].stock+=static_cast<double>(a.amount)/w.config.medicalCost;}
            }
        }
        w.event(EventType::PolicyApplied,0,"Validated political action "+std::to_string(static_cast<int>(a.type))+" applied");
        return true;
    }
}
