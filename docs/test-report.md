> Rapport historique V1. Pour la livraison jouable, voir [validation V1.1](test-report-v1.1.md).

# Rapport de validation réellement exécutée

Validation sous WSL Ubuntu, G++, C++17, GNU Make. Tous les résultats ci-dessous proviennent des commandes exécutées dans le dépôt, pas de valeurs attendues inventées. Les logs bruts et métadonnées sont conservés dans `docs/validation/`. Les durées dépendent de la machine; ce ne sont pas des garanties de performance.

## Résultat

**26 exécutables de tests réussis**, dont les six tests d'origine inchangés. Compilation propre avec `-Wall -Wextra -Wpedantic`, sans avertissement dans les logs de la version finale. Les commandes par moteur ont réussi. L'ultime ajustement de mise en forme a été suivi de `make -s -j4 all test`, réussi; voir [sortie finale](validation/final-build-test.log).

| Commande | Code de sortie | Durée observée | Preuve |
|---|---:|---:|---|
| `make clean` | 0 | 0.09 s | [sortie](validation/clean.log) |
| `make -j4` | 0 | 7.79 s | [sortie](validation/build.log) |
| `make test` | 0 | 19.44 s | [sortie](validation/test.log) |
| `make test-economy test-society test-politics test-military test-disasters test-simulation` | 0 | 0.12 s | [sortie](validation/groups.log) |
| `make run` | 0 | 0.13 s | [sortie](validation/run.log) |
| `./build/dictatorsim --population 200 --days 3600 --seed 42 --ledger-log build/long-transactions.csv` | 0 | 0.25 s | [sortie](validation/long.log) |
| `./build/dictatorsim --population 10000 --days 365 --seed 42 --ledger-log build/large-transactions.csv` | 0 | 5.35 s | [sortie](validation/large.log) |
| `./build/dictatorsim --population 0 --days 720 --seed 42` | 0 | 0.0 s | [sortie](validation/empty.log) |
| `./build/dictatorsim --population 10000 --days 3650 --seed 42 --ledger-log /dev/null` | 0 | 14.49 s | [sortie](validation/stress.log) |

Le test de charge 10000 citoyens / 3650 jours utilise `/dev/null` comme destination de journal pour ne pas conserver plusieurs gigaoctets de transactions; les transferts et invariants ont bien été exécutés, mais le détail transactionnel de cette seule exécution n'est pas conservé. Le scénario 10000 / 365 écrit réellement `build/large-transactions.csv`; ArchiveTest vérifie par ailleurs 100005 lignes, leurs identifiants et la rotation mémoire.

## Tests exécutés

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
- `build/tests/ExceptionalPaymentTest`
- `build/tests/HappinessSystemTest`
- `build/tests/IdGeneratorTest`
- `build/tests/IncomeSystemTest`
- `build/tests/LedgerTest`
- `build/tests/MilitaryEngineTest`
- `build/tests/PolicyExecutorTest`
- `build/tests/PopulationManagerTest`
- `build/tests/ProtestSystemTest`
- `build/tests/RandomEngineTest`
- `build/tests/RebellionSystemTest`
- `build/tests/SimulationClockTest`
- `build/tests/SimulationIntegrationTest`
- `build/tests/TaxationSystemTest`

Couverture économique : validité/refus/solvabilité, bornes, conservation, 100000 transferts reproductibles, salaire, allocation, achat et livraison, impôt et remise à zéro de l'assiette, soins et réparation, facture reportée, vol/restitution/amende, dégâts physiques et trésorerie. Les deux instances du test de reproductibilité comparent montants, comptes, statuts et types de chaque transaction retenue, ainsi que les principaux agrégats. Ce n'est pas une preuve formelle couvrant toutes les graines possibles.

Couverture intégration : 1080 jours avec événements, 720 jours sans population, 720 jours sans monnaie, flux naturels inchangés lorsque la fréquence criminelle change, commande humaine validée, validation hybride, configurations invalides (dont somme de parts proches d'INT_MAX). World vérifie quotidiennement la monnaie, les bornes des états, les stocks, capacités et la correspondance id→citoyen.

## Résultats observés des scénarios

### run

```text
DAY: 365 YEAR: 2
POPULATION: 1000
Money supply: 10000000.00
Treasury: 1336623.41
Happiness: 93.8409
Stability: 98.4459
Military loyalty: 94.8196
Difference: 0.00
STATUS: CONSISTENT
```

### long

```text
DAY: 3600 YEAR: 11
POPULATION: 200
Money supply: 10000000.00
Treasury: 43591.10
Happiness: 11.3
Stability: 24.875
Military loyalty: 50
Difference: 0.00
STATUS: CONSISTENT
```

### large

```text
DAY: 365 YEAR: 2
POPULATION: 10000
Money supply: 10000000.00
Treasury: 2295595.46
Happiness: 41.2677
Stability: 33.8526
Military loyalty: 80.9809
Difference: 0.00
STATUS: CONSISTENT
```

### empty

```text
DAY: 720 YEAR: 3
POPULATION: 0
Money supply: 10000000.00
Treasury: 6000000.00
Happiness: 0
Stability: 25
Military loyalty: 0
Difference: 0.00
STATUS: CONSISTENT
```

### stress

```text
DAY: 3650 YEAR: 11
POPULATION: 10000
Money supply: 10000000.00
Treasury: 883.80
Happiness: 11.1228
Stability: 7.9129
Military loyalty: 0.0846687
Difference: 0.00
STATUS: CONSISTENT
```

## Limites de validation

Les scénarios de crise sont des résultats du modèle, pas des échecs comptables. Aucune estimation empirique, validation sociologique, analyse de sensibilité exhaustive ou étude de distribution des issues politiques n'a été réalisée. Pas d'exécution ASan/TSan, ni de preuve de sûreté multi-thread; le moteur est mono-thread. Pas de test graphique, de réseau ou de LLM puisqu'ils ne font pas partie de cette version. Les interventions politiques agrégées, le recrutement, les ressources intermédiaires et les contrats préparatoires restent signalés PARTIAL/CONTRACT_ONLY dans l'inventaire.

Les six anciens binaires déjà suivis par Git ont été restaurés à leur contenu initial après validation afin de ne pas livrer de diff binaire. Les exécutables canoniques nouvellement compilés sont `build/tests/*Test`; toutes les cibles de test utilisent ceux-ci. Aucun test source préexistant n'a été supprimé ou modifié. Aucun commit n'a été créé.
