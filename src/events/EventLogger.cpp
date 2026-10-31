#include "dictatorsim/events/EventLogger.hpp"
namespace ds {
    void EventLogger::collect(EventBus& b) {
        for(auto& e:b.drain())events_.push_back(std::move(e));
    }
    const std::vector<Event>& EventLogger::entries()const {
        return events_;
    }
    void EventLogger::print(std::ostream& out,std::size_t last)const {
        auto start=events_.size()>last?events_.size()-last:0;
        for(auto i=start;i<events_.size();++i)out<<"Day "<<events_[i].day<<": "<<events_[i].message<<" (subject "<<events_[i].subject<<")\n";
    }
}
