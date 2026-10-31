#include "dictatorsim/events/PlayerNotification.hpp"
namespace ds {
PlayerNotification EventClassifier::classify(const Event& e) {
    PlayerNotification n; n.day=e.day;n.message=e.message;
    switch(e.type) {
    case EventType::MilitaryCoupStarted:n.category=EventCategory::Military;n.severity=EventSeverity::Critical;n.requiresDecision=true;break;
    case EventType::GovernmentCollapsed:case EventType::RebellionStarted:n.category=EventCategory::Politics;n.severity=EventSeverity::Critical;n.requiresDecision=true;break;
    case EventType::EarthquakeOccurred:case EventType::TsunamiOccurred:case EventType::VolcanicEruptionOccurred:case EventType::DroughtStarted:case EventType::FloodOccurred:
        n.category=EventCategory::Disaster;n.severity=EventSeverity::Major;n.requiresDecision=true;break;
    case EventType::PoliticalScandal:n.category=EventCategory::Politics;n.severity=EventSeverity::Major;n.requiresDecision=true;break;
    case EventType::ProtestStarted:n.category=EventCategory::Society;n.severity=EventSeverity::Important;break;
    case EventType::CrimeOccurred:case EventType::CitizenBecameSick:n.category=EventCategory::Society;break;
    case EventType::PolicyApplied:n.category=EventCategory::Politics;break;
    default:break;
    }
    return n;
}
}
