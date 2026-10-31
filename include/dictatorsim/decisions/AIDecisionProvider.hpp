#pragma once
#include "IDecisionProvider.hpp"
namespace ds {
    // Offline structured proposal contract. A future adapter implements request/poll.
    class AIDecisionProvider: public IDecisionProvider {
        public: ~AIDecisionProvider() override=default;
    };
}
