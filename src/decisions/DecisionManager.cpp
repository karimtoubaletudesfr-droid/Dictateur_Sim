#include "dictatorsim/decisions/DecisionManager.hpp"
#include "dictatorsim/politics/PolicyExecutor.hpp"
#include "dictatorsim/core/World.hpp"
namespace ds {
    void DecisionManager::setProvider(IDecisionProvider* p) {
        provider_=p;
    }
    void DecisionManager::update(World& w,const GovernmentReport& report) {
        if(!provider_)return;
        provider_->request({requestId_++,report});
        auto result=provider_->poll();
        if(result && result->approved && result->requestId>=minimumResult_ && result->requestId<requestId_) {
            minimumResult_=result->requestId+1;
            PolicyExecutor{}.execute(w,result->action);
        }
    }
}
