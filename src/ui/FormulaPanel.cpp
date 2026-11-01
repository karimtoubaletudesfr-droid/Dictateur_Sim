#include "dictatorsim/ui/FormulaPanel.hpp"
#include <array>
namespace ds {
namespace {
struct Formula {const char* title;const char* equation;const char* explanation;const char* source;};
// This reference follows the engine equations, including bounds and time scales.
const std::array<Formula,18> formulas{{
 {"01 / Bonheur individuel","cible = (a*repas + b*logement + c*sante + d*(100-fatigue)) / (a+b+c+d) - 0.15*stress\nbonheur' = borne(0.9*bonheur + 0.1*cible)","a, b, c, d : poids configurables des besoins. Inertie quotidienne : 90 % de l'ancien bonheur est conserve.","society/systems/HappinessSystem.cpp"},
 {"02 / Fatigue et stress","fatigue' = borne(fatigue + charge - repos)\nstress' = borne(stress + frais + effort)","Charge = fatigue config. * charge physique * heures / 8 au travail (sauf jour 7), sinon 0. Frais : +1 si dette de service, sinon -0.5. Effort : 0.2 * charge mentale au travail, sinon -1.","society/systems/HappinessSystem.cpp"},
 {"03 / Nourriture et sante","repas' = borne(repas + (achat reussi ? 20 : -25))\nsante' = borne(sante + (repas < 30 ? -0.5 : 0.1) - soins)","Achat quotidien : stock et argent necessaires. Soins = 0.2 si traitement medical impaye, sinon 0. La maladie retire aussi 20 points lors de son apparition ; un soin paye rend 15 points.","economy/systems/ConsumptionSystem.cpp ; society/systems/HappinessSystem.cpp"},
 {"04 / Salaire, impot et aides","impot = entier(revenu gagne * taux / 10000)\naide individuelle = entier(entier(Tresor * taux aides / 10000) / beneficiaires)","Chaque fin de mois, salaire du metier si l'employeur peut payer. Les soldats dependent du budget militaire. Aides aux sans-employeur hors soldats ; aucun beneficiaire : paiement nul. Argent calcule en centimes.","economy/systems/WageSystem.cpp ; TaxationSystem.cpp ; IncomeSystem.cpp"},
 {"05 / Production et stock","travail = somme(productivite metier * sante/100 * (1-fatigue/150))\nproduction = min(travail * productivite entreprise * capacite * climat, place)","Place = max(0, travail * productivite entreprise * 60 - stock). Climat = multiplicateur de secheresse pour l'agriculture, sinon 1. Capacite diminue lors des catastrophes.","economy/systems/ProductionSystem.cpp ; economy/Company.cpp"},
 {"06 / Prix et inflation","pas = max(1 centime, entier(prix / 50))\nindice = somme(prix) / somme(prix de base) ; inflation = indice / ancien indice - 1","Fin de mois : prix + pas si stock < population ; prix - pas si stock > 15 * population. Bornes : base/10 (au moins 1 centime) a base*10. L'inflation est la variation de l'indice entre deux calculs.","economy/systems/PricingSystem.cpp ; InflationSystem.cpp"},
 {"07 / Confiance et colere","confiance' = borne(confiance + 0.01*(bonheur-50) - 0.15*Gini - restrictions)\ncolere' = borne(0.95*colere + 0.05*(0.45*(100-confiance') + 0.3*(100-repas) + 25*Gini))","Restrictions : +0.1 sous etat d'urgence, +0.05 pour la censure, +0.05 pour une organisation interdite. La colere depend de la confiance et des besoins : ce n'est pas 100 - bonheur.","society/systems/HappinessSystem.cpp"},
 {"08 / Inegalites de patrimoine","Gini = somme((2*i+1)*solde[i]) / (N*somme(soldes)) - 1","Soldes des citoyens tries par ordre croissant, indices i de 0 a N-1. Si la somme est nulle, Gini = 0. Il mesure les inegalites d'argent disponible, pas celles des salaires.","society/systems/InequalitySystem.cpp"},
 {"09 / Tentative de vol","P = base crime * (0.1 + 0.5*(100-repas)/100 + 0.4*gout du risque)\n    * (1 - 0.5*police)","Tirage quotidien par citoyen (au moins deux citoyens). Succes : 0.7*(1-0.5*police). Signalement : police. Montant vole limite au cout de reparation et au solde de la victime.","society/systems/CrimeSystem.cpp"},
 {"10 / Entrer en manifestation","P = borne01(base * colere/100 * (1-confiance/100) * (0.5+participation)\n    * (1+radicalisation/100) * maintien * (1-peur/150))","Participation : part de manifestants au jour precedent. Maintien : 0.25 sous etat d'urgence, sinon 0.7. Pour un manifestant actuel, chance de quitter = 0.03 + 0.1*confiance/100.","society/systems/ProtestSystem.cpp"},
 {"11 / Rebellion","P = base rebellion * participation * colere moyenne/100 * (1-legitimite/100)","Tirage seulement sans rebellion active, regime non effondre, et participation >= seuil configure (%). Une rebellion active a 2 % de chance quotidienne de resolution ; la loyaute influence son issue.","society/systems/RebellionSystem.cpp"},
 {"12 / Catastrophes et incidents","P catastrophe(region, type) = base catastrophe * risque regional\nP maladie eligible = (1 - base panne) * base maladie","15 tirages de catastrophe par jour : 3 regions x 5 types. Panne testee avant maladie, uniquement sans frais en attente. Les risques regionaux peuvent etre nuls : la base seule ne determine pas la frequence.","disasters/DisasterSystem.cpp ; economy/systems/LivingCostSystem.cpp"},
 {"13 / Legitimite et stabilite","legitimite = moyenne(confiance des citoyens)\nstabilite = borne(0.45*legitimite + 0.30*loyaute + 0.25*(100-mobilisation) - crise)","Mobilisation = 100 * manifestants / population (0 si vide). Crise = 25 si rebellion active, sinon 0. Sans citoyen, la legitimite vaut 0.","politics/systems/LegitimacySystem.cpp ; StabilitySystem.cpp"},
 {"14 / Loyaute militaire","cible = 0.50*satisfaction paie + 0.25*bonheur soldats + 0.25*legitimite\nloyaute' = borne(0.98*loyaute + 0.02*cible)","Satisfaction paie = 100 * soldats payes / effectif, actualisee a la fin du mois. Bonheur soldats : moyenne de leurs bonheurs. Sans soldat, loyaute et satisfaction valent 0.","military/systems/LoyaltySystem.cpp"},
 {"15 / Coup d'Etat","P = base putsch * (1-loyaute/100) * (1-legitimite/100) * (1-stabilite/100)","Nouveau tirage seulement avec soldats, regime non effondre, aucun putsch en attente et loyaute <= seuil configure. En mode joueur, la menace attend votre reponse avant sa resolution.","military/systems/CoupSystem.cpp"},
 {"16 / Chomage et prosperite","chomage = 100 * sans-emploi / actifs\nprosperite = borne(50 * achats alimentaires / population + 50*(1-chomage/100))","Actifs : citoyens hors etudiants et retraites. Sans-emploi : pas d'employeur et pas soldat. Les ratios valent 0 quand leur denominateur est nul ; prosperite = 0 sans population.","statistics/MetricsCalculator.cpp"},
 {"17 / Comptabilite fermee","masse monetaire = somme(soldes citoyens + entreprises + Tresor)\nmasse monetaire = masse initiale","Salaires, achats, aides, impots et vols sont des transferts. Aucun de ces mecanismes ne cree d'argent. Un paiement peut etre refuse faute de fonds : un montant configure n'est pas une promesse de versement.","economy/Ledger.cpp ; core/World.cpp"},
 {"18 / Lecture des visages sur la carte","Faim si repas < 35 ; sinon fatigue si fatigue >= 65 ; sinon colere si colere >= 60\nsinon content si bonheur >= 60 ; sinon neutre","Priorite d'affichage, pas une nouvelle loi du moteur. De 48 a 800 visages au choix, limites a la population reelle. Echantillonnage regulier. Survolez un visage pour voir toutes ses valeurs. Positions schematiques.","ui/UrbanMap.cpp"}
}};
}
void FormulaPanel::draw(sf::RenderTarget& t,UITheme& ui,const PlayerSession& session,sf::Vector2f mouse,bool click){
    const sf::Color paper(237,231,213),ink(42,57,56),secondary(93,109,103),gold(203,175,112);
    ui.panel(t,"formula-bg",{0,82,1280,590},{22,40,42});
    ui.text(t,"formula-kicker","L'ATELIER DU GOUVERNEMENT",32,104,12,gold);
    ui.text(t,"formula-title","Les chiffres ont une histoire.",32,125,27,ui.ink);
    ui.text(t,"formula-intro","Les equations du simulateur, expliquees.  ' = valeur suivante ; borne = entre 0 et 100 ; borne01 = entre 0 et 1.",32,163,13,ui.muted);
    const char* categories[]={"01  Vie quotidienne","02  Economie","03  Societe","04  Crises & risques","05  Pouvoir & armee","06  Lire les indicateurs"};
    for(int i=0;i<6;++i)if(ui.button(t,"formula-category"+std::to_string(i),categories[i],{28,207+i*52.f,238,42},mouse,click,category_==i))category_=i;
    ui.text(t,"formula-note","Probabilites : 0 a 1.\n0.02 signifie 2 %.\n\nLes indicateurs evoluent\ndans l'ordre des systemes,\npas simultanement.",38,535,12,ui.muted);
    for(int row=0;row<3;++row){const auto& f=formulas[category_*3+row];float y=201+row*150.f;std::string key="formula-card"+std::to_string(row);
        ui.roundPanel(t,{285,y+3,967,143},{0,0,0,60},9);ui.roundPanel(t,{281,y,967,143},paper,9);
        ui.text(t,key+"-title",f.title,299,y+10,17,ink);
        ui.text(t,key+"-equation",f.equation,299,y+36,12,{28,106,94});
        ui.text(t,key+"-explanation",ui.wrap(f.explanation,126),299,y+73,11,secondary);
        ui.text(t,key+"-source",std::string("Source : src/")+f.source,299,y+124,9,secondary);
    }
    ui.text(t,"formula-day","REFERENCE DU MOTEUR  /  JOUR "+std::to_string(session.simulation().world().clock.day()),920,122,12,gold);
}
}
