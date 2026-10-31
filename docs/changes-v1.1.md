# Changements principaux V1.1

- Ajouts : `core/PlayerSession`, `decisions/PlayerDecisionController`, `DecisionHistory`, `DecisionOption`, classification/notifications d'événements, conséquences politiques différées.
- Ajouts UI : `apps/sfml/main.cpp`, les sept headers UI et leurs implémentations dans `src/ui/` (y compris le thème partagé).
- Extensions ciblées : `PoliticalAction`, `PoliticalState`, `Policy`, `ActionValidator`, `PolicyExecutor`, `Simulation`, snapshots/statistiques, systèmes sociaux, coup d'État, travaux de logement. Aucun remplacement de World, Ledger ou PopulationManager.
- Build : cœur isolé de SFML, cibles `sfml`, `run-sfml`, `test-player` et dépendances automatiques UI.
- Tests : cinq nouveaux fichiers `PlayerSessionTest.cpp`, `PlayerDecisionImpactTest.cpp`, `PlayerActionsTest.cpp`, `PlayerCoupTest.cpp`, `EventSeverityTest.cpp`; tous les tests antérieurs conservés.
- Documentation : README, player-mode, political-actions, sfml-interface, event-severity, decision-impact, audit-v1.1, test-report-v1.1 et preuves dans validation-v1.1.

Les contrats IA/hybride préexistants restent inutilisés; aucune fonctionnalité IA nouvelle. Aucun commit ni réécriture d'historique Git.
