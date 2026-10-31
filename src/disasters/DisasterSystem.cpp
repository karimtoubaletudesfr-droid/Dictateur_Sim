#include "dictatorsim/disasters/DisasterSystem.hpp"
#include "dictatorsim/core/World.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace ds {
    void DisasterSystem::apply(World& w,const DisasterEvent& e)const {
        if(e.region>2 || !std::isfinite(e.severity)||e.severity<0||e.severity>1)throw std::invalid_argument("invalid disaster");
        if(static_cast<unsigned>(e.type)>4)throw std::invalid_argument("invalid disaster type");
        w.disasters.push_back(e);
        if(e.type==DisasterType::Drought) {
            w.economy.agriculturalMultiplier=1-e.severity;
            w.economy.droughtUntil=w.clock.day()+60;
        }
        else for(auto& c:w.companies)if(c.id%3==e.region)c.damage(e.severity);
        if(e.type!=DisasterType::Drought)for(const auto& item:w.population.getPopulation())if(item.getId()%3==e.region) {
            auto& p=*w.population.findCitizen(item.getId());
            p.housing=std::max(0.f,p.housing-static_cast<float>(60*e.severity));
            p.health=std::max(0.f,p.health-static_cast<float>(20*e.severity));
            if(p.pendingCost==0) {
                p.pendingCost=w.config.repairCost;
                p.pendingService=3;
            }
        }
        EventType type=EventType::EarthquakeOccurred;
        switch(e.type) {
            case DisasterType::Tsunami:type=EventType::TsunamiOccurred;
            break;
            case DisasterType::Drought:type=EventType::DroughtStarted;
            break;
            case DisasterType::Flood:type=EventType::FloodOccurred;
            break;
            case DisasterType::VolcanicEruption:type=EventType::VolcanicEruptionOccurred;
            break;
            default:break;
        }
        w.event(type,e.region,"Natural disaster type "+std::to_string(static_cast<int>(e.type))+" damaged physical assets; severity "+std::to_string(e.severity));
    }
    void DisasterSystem::update(World& w)const {
        // Three fictional regions: coastal plain, inland agricultural area, volcanic highland.
        const auto& risks=w.config.regionalRisks;
        for(unsigned region=0;region<3;++region)for(unsigned type=0;type<5;++type)
        if(w.disasterRandom.probability(w.config.disasterProbability*risks[region][type]))apply(w,{static_cast<DisasterType>(type),region,w.disasterRandom.randomFloat(.1f,.6f),w.clock.day()});
    }
}
