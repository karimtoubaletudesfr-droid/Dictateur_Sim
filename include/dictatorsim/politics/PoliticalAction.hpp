#pragma once
#include "dictatorsim/economy/Money.hpp"
namespace ds {
    enum class ActionType {
        IncreaseTaxes, DecreaseTaxes, IncreaseMilitaryBudget, IncreaseEducationBudget, IncreaseHealthcareBudget, SubsidizeFood, BuildHospital, InvestInIndustry, DeclareEmergency, IncreasePoliceBudget, ProvideDisasterRelief, DecreasePublicSpending, AllowProtest, NegotiateProtest, CensorMedia, BanOrganization, ReleasePrisoners, ArrestOpponent, PoliticalAssassination, ViolentRepression, DestroyLocality, EndEmergency, NoAction
    };
    enum class ActionVisibility { Public, Covert };
    struct PoliticalAction {
        ActionType type;
        Money amount=0;
        int basisPoints=0;
        ActionVisibility visibility=ActionVisibility::Public;
        unsigned target=0; // Fictional locality 0..2; named opposition figure otherwise.
    };
}
