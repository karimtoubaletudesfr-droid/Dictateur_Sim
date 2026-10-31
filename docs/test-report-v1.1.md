# Validation exécutée — V1.1

Compilation C++17/GNU Make sous WSL Ubuntu. SFML 2.6.1 détecté, serveur WSLg réellement interrogé avec xdpyinfo. Aucun avertissement compilateur dans les logs finaux. Les fichiers de tests V1 restent présents. Aucun commit créé.

## Commandes exécutées

| Commande | Code | Durée totale observée | Preuve |
|---|---:|---:|---|
| `make clean` | 0 | 0.06 s | [clean](validation-v1.1/clean.log) |
| `make -j4` | 0 | 7.76 s | [core-build](validation-v1.1/core-build.log) |
| `make -j4 test` | 0 | 7.55 s | [tests](validation-v1.1/tests.log) |
| `make -j4 sfml` | 0 | 3.36 s | [sfml-build](validation-v1.1/sfml-build.log) |
| `make run` | 0 | 0.14 s | [cli](validation-v1.1/cli.log) |
| `make test-player` | 0 | 0.07 s | [player-tests](validation-v1.1/player-tests.log) |
| `./build/dictatorsim-sfml --smoke 120 --width 1280 --height 720` | 0 | 2.57 s | [smoke-720](validation-v1.1/smoke-720.log) |
| `./build/dictatorsim-sfml --smoke 120 --width 1920 --height 1080` | 0 | 2.69 s | [smoke-1080](validation-v1.1/smoke-1080.log) |
| `./build/dictatorsim-sfml --benchmark 240 --population 1000 --width 1920 --height 1080` | 0 | 4.74 s | [benchmark-1000](validation-v1.1/benchmark-1000.log) |
| `./build/dictatorsim-sfml --benchmark 240 --population 10000 --width 1280 --height 720` | 0 | 4.4 s | [benchmark-10000](validation-v1.1/benchmark-10000.log) |

## Tests logiques

**31 exécutables réussis**, dont les 26 tests V1. Les cinq nouveaux tests :

- PlayerSessionTest : pause, reprise, quatre vitesses, retard conservé, plafond de pas/frame, aucune décision autonome, refus, réponse impossible, cooldown, crises simultanées et interdiction des fournisseurs automatiques.

- EventSeverityTest : quatre gravités et décision requise ou informative.

- PlayerDecisionImpactTest : changement fiscal réel sur paiements, financement sanitaire payé et traitement effectif, observation à trente jours, reproductibilité.

- PlayerActionsTest : assertions sur les effets propres de chacune des 22 actions, conservation, dommages physiques, victimes, action secrète et révélation reproductible.

- PlayerCoupTest : menace différée, attente de réponse, remplacement du dirigeant; une partie sans ressources perd effectivement le pouvoir puis refuse temps et nouveaux ordres.

Les anciens tests continuent à couvrir notamment 100000 transactions reproductibles, stocks, salaires, impôts, vols/restitutions, population vide et simulations pluriannuelles. Le lien du [CLI](validation-v1.1/cli-linkage.log) ne contient aucune bibliothèque SFML.

## Validation graphique réelle

Deux fenêtres SFML ont été exécutées, à 1280×720 et 1920×1080. Le mode de test injecte des coordonnées de souris dans le code normal des boutons : 1x, pause, revue/confirmation fiscale, modal de crise, réponse sans action, historique, menu et retour. Ce sont des interactions automatisées sur une vraie fenêtre; pas un essai manuel exhaustif de toutes les combinaisons utilisateur.

Le taux fiscal est passé de 1500 à 2000 points de base; deux décisions (ordre fiscal et réponse sans action) sont enregistrées. Le temps reste en pause, le bilan monétaire est conservé. Captures inspectées : [tableau de bord](validation-v1.1/dashboard-720.png), [crise](validation-v1.1/crisis-720.png), [historique](validation-v1.1/history-720.png), [1080p](validation-v1.1/dashboard-1080.png).

## Mesures de performance

- benchmark-1000 : 240 frames / 4.65604 secondes = **51.5 FPS moyens**, 0.043128 secondes cumulées dans la logique, jour atteint 29. Zéro décision politique exécutée, taux fiscal inchangé, monnaie conservée.

- benchmark-10000 : 240 frames / 4.31428 secondes = **55.6 FPS moyens**, 0.383233 secondes cumulées dans la logique, jour atteint 29. Zéro décision politique exécutée, taux fiscal inchangé, monnaie conservée.

Ces mesures incluent le coût des captures PNG. Les crises naturelles ont arrêté la simulation au jour 29; ce ne sont donc pas des mesures de plusieurs minutes à charge logique constante. Le limiteur vise 60 FPS, sans garantie de 60 sur cette machine WSLg. Le message SFML « Setting vertical sync not supported » apparaît : le limiteur logiciel est utilisé. Aucun blocage ni plantage observé dans ces essais.

## Limites connues

UI en anglais; pas de sauvegarde/reprise; pas de simulation judiciaire, de détenus individualisés ou de mortalité démographique liée aux actions. Le financement public et les localités restent agrégés. Les coups et la radicalisation sont des règles fictives, non calibrées. Pas d’IA, de serveur, de modèle réseau ou de mode hybride actif. Le jeu est mono-thread et une journée exceptionnellement coûteuse peut retarder une frame. Aucun objectif de performance n’est présenté comme garanti.

## Liste exacte des tests

- `build/tests/AccountTest`

- `build/tests/ArchiveTest`

- `build/tests/CSVProfessionProviderTest`

- `build/tests/CitizenFactoryTest`

- `build/tests/CitizenTest`

- `build/tests/CompanyTest`

- `build/tests/ConfigurationTest`

- `build/tests/ConsumptionSystemTest`

- `build/tests/CrimeSystemTest`

- `build/tests/DecisionManagerTest`

- `build/tests/DisasterSystemTest`

- `build/tests/EconomyEngineTest`

- `build/tests/EventSeverityTest`

- `build/tests/ExceptionalPaymentTest`

- `build/tests/HappinessSystemTest`

- `build/tests/IdGeneratorTest`

- `build/tests/IncomeSystemTest`

- `build/tests/LedgerTest`

- `build/tests/MilitaryEngineTest`

- `build/tests/PlayerActionsTest`

- `build/tests/PlayerCoupTest`

- `build/tests/PlayerDecisionImpactTest`

- `build/tests/PlayerSessionTest`

- `build/tests/PolicyExecutorTest`

- `build/tests/PopulationManagerTest`

- `build/tests/ProtestSystemTest`

- `build/tests/RandomEngineTest`

- `build/tests/RebellionSystemTest`

- `build/tests/SimulationClockTest`

- `build/tests/SimulationIntegrationTest`

- `build/tests/TaxationSystemTest`
