# Audit V1.1

README, Makefile, CLI, Simulation, World, horloge, gouvernement, politiques, validateurs, EventBus, statistiques, Ledger et PopulationManager ont été relus avant modification. Les modifications V1 non commitées et les binaires déjà modifiés étaient présents au début; ils n'ont pas été annulés.

Points réutilisés : `Simulation::step`, `act`, `report`, `world` const; `ActionValidator::validate`, `PolicyExecutor::execute`, comptes Ledger; journal EventLogger et snapshots StatisticsEngine. Aucune réécriture de moteur. Le CLI reste un exécutable distinct.

Adaptations ciblées : `Simulation::act` rafraîchit immédiatement les observations; `playerMode` interdit l'attachement d'un fournisseur automatique et diffère le résultat des coups d'État. PoliticalAction est étendu (visibilité/cible/types). DecisionRequest/Result sont étendus en conservant les initialisations V1. Une couche PlayerSession orchestre uniquement temps, notifications et décisions humaines. Les nouveaux effets politiques restent dans politics et les effets quotidiens dans les moteurs existants.

SFML 2.6.1 détecté par pkg-config; serveur Microsoft WSLg vérifié par xdpyinfo, pas uniquement par la présence de DISPLAY. Police système présente. Les sources UI sont exclues du lien logique et compilées uniquement par la cible graphique.
