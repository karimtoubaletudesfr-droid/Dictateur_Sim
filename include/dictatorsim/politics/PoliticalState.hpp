#pragma once
namespace ds {
    enum class RegimeStatus {
        Governing, Transition, Collapsed, Crisis
    };
    struct PoliticalState {
        double legitimacy=50, stability=70;
        RegimeStatus regime=RegimeStatus::Governing;
        unsigned leadershipChanges=0;
        double fear=0, radicalization=0;
        unsigned victims=0, prisoners=0, scandals=0;
        bool opponentAlive=true, opponentImprisoned=false;
        bool pendingCoup=false, coupResponseReady=false;

    };
}
