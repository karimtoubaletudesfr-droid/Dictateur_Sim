#include "TestWorld.hpp"
#include "dictatorsim/events/PlayerNotification.hpp"
int main(){
    auto price=ds::EventClassifier::classify({ds::EventType::PricesChanged,1,0,"price"});assert(price.severity==ds::EventSeverity::Minor && !price.requiresDecision);
    auto protest=ds::EventClassifier::classify({ds::EventType::ProtestStarted,1,0,"protest"});assert(protest.severity==ds::EventSeverity::Important && !protest.requiresDecision);
    auto drought=ds::EventClassifier::classify({ds::EventType::DroughtStarted,1,0,"drought"});assert(drought.severity==ds::EventSeverity::Major && drought.requiresDecision);
    auto coup=ds::EventClassifier::classify({ds::EventType::MilitaryCoupStarted,1,0,"coup"});assert(coup.severity==ds::EventSeverity::Critical && coup.requiresDecision);
}
