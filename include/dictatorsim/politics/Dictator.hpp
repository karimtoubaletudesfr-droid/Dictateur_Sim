#pragma once
namespace ds {
    struct Personality {
        double ego=.5, paranoia=.5, greed=.5, intelligence=.5, empathy=.5, authoritarianism=.5, riskTolerance=.5, ambition=.5;
    };
    class Dictator {
        Personality personality_;
        public: explicit Dictator(Personality p={});
        const Personality& personality() const;
    };
}
