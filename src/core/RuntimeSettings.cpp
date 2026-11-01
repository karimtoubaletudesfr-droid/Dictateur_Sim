#include "dictatorsim/core/RuntimeSettings.hpp"
#include "dictatorsim/core/World.hpp"
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
namespace ds {
const std::vector<RuntimeParameter>& runtimeParameters(){
    static const auto fields=[] {
        std::vector<RuntimeParameter> v;
        auto add=[&](const char* category,const char* label,const char* description,const char* unit,auto member,double lo,double hi,double step,double scale=1.){
            v.push_back({category,label,description,unit,lo,hi,step,
                [member,scale](const SimulationConfig& c){return double(c.*member)/scale;},
                [member,scale](SimulationConfig& c,double value){using T=std::decay_t<decltype(c.*member)>;c.*member=static_cast<T>(std::is_integral_v<T>?std::round(value*scale):value*scale);}});
        };
        const char* names[]={"Sans emploi","Etudiant","Retraite","Agriculteur","Boulanger","Enseignant","Chercheur","Ingenieur","Medecin","Ouvrier","Soldat"};
        for(int i=3;i<11;++i)v.push_back({"Salaires",names[i],i==10?"Solde mensuelle payee par le Tresor, dans la limite du budget militaire.":"Salaire mensuel verse par l'employeur si ses fonds sont suffisants. Prochaine paie.","argent / mois",0,10000000,50,
            [i](const SimulationConfig& c){return c.salaries[i]/100.;},[i](SimulationConfig& c,double x){c.salaries[i]=std::llround(x*100);}});
        add("Evenements","Criminalite","Chance de base par citoyen et par jour, ajustee par le besoin et la police.","%",&SimulationConfig::crimeProbability,0,100,.1,.01);
        add("Evenements","Maladie","Chance quotidienne par citoyen de subir des frais medicaux.","%",&SimulationConfig::sicknessProbability,0,100,.01,.01);
        add("Evenements","Panne","Chance quotidienne par citoyen de subir des frais de reparation.","%",&SimulationConfig::breakdownProbability,0,100,.01,.01);
        add("Evenements","Catastrophe","Chance de base quotidienne de catastrophe ; le risque regional intervient aussi.","%",&SimulationConfig::disasterProbability,0,100,.1,.01);
        add("Evenements","Manifestation","Chance de base de mobilisation, ajustee par colere, confiance et peur.","%",&SimulationConfig::protestProbability,0,100,1,.01);
        add("Evenements","Rebellion","Chance de base quotidienne, ajustee par mobilisation, colere et legitimite.","%",&SimulationConfig::rebellionProbability,0,100,1,.01);
        add("Evenements","Coup d'Etat","Chance de base quotidienne, ajustee par loyaute, legitimite et stabilite.","%",&SimulationConfig::coupProbability,0,100,1,.01);
        add("Evenements","Corruption","Chance de base de corruption, modulee par l'etat politique.","%",&SimulationConfig::corruptionProbability,0,100,.1,.01);
        add("Seuils","Mobilisation minimale","Part minimale de manifestants pour permettre une nouvelle rebellion. 0 : aucun minimum.","% citoyens",&SimulationConfig::rebellionParticipation,0,100,1);
        add("Seuils","Loyaute maximale pour un putsch","Un nouveau coup d'Etat est possible seulement sous ce plafond de loyaute.","/ 100",&SimulationConfig::coupLoyaltyThreshold,0,100,1);
        add("Economie","Cout de la vie","Cout de reference utilise pour les besoins et certaines sanctions. Prochain calcul.","argent",&SimulationConfig::livingCost,.01,10000000,1,100);
        add("Economie","Reparations","Montant d'une reparation ; sert aussi de plafond pour un vol. Prochain evenement.","argent",&SimulationConfig::repairCost,.01,10000000,10,100);
        add("Economie","Soins medicaux","Montant des soins lors d'une maladie. Prochain evenement.","argent",&SimulationConfig::medicalCost,.01,10000000,10,100);
        add("Economie","Fatigue","Coefficient de fatigue liee au travail. Applique au prochain jour simule.","coefficient",&SimulationConfig::fatigue,0,100,.1);
        add("Economie","Repos","Coefficient de recuperation quotidienne. Applique au prochain jour simule.","coefficient",&SimulationConfig::rest,0,100,.1);
        return v;
    }();return fields;
}
void World::applySettings(const SimulationConfig& settings){
    auto next=config;
    for(const auto& f:runtimeParameters()){
        double value=f.get(settings);
        if(!std::isfinite(value)||value<f.minimum||value>f.maximum)throw std::invalid_argument("Valeur invalide : "+f.label);
        f.set(next,value);
    }
    next.wage=next.salaries[10];
    // A zero military salary is allowed; the legacy general wage must stay positive.
    next.wage=std::max<Money>(1,next.wage);
    next.validate();config=next;
    for(auto& profession:professions_)profession.referenceSalary=config.salaries[static_cast<std::size_t>(profession.profession)];
}
}
