#include "dictatorsim/decisions/DecisionHistory.hpp"
namespace ds {
void DecisionHistory::add(DecisionRecord r){records_.push_back(std::move(r));}
void DecisionHistory::observe(const StatisticsSnapshot& s){for(auto& r:records_)if(r.accepted && !r.afterThirtyDays && s.day>=r.day+30)r.afterThirtyDays=s;}
const std::vector<DecisionRecord>& DecisionHistory::records()const{return records_;}
}
