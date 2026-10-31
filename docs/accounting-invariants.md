# Invariants comptables

## Définition exacte

`Money = std::int64_t`, centimes. M(t) = somme des `Account::balance_` réellement possédés par Ledger. Tous les comptes citoyens, entreprises et Trésor font partie du périmètre. Treasury, Citizen et Company ne répliquent pas ce solde. `initial_` est une référence de contrôle, jamais le résultat retourné par `moneySupply()`.

L'ouverture alloue des montants positifs ou nuls dont la somme ne dépasse pas INT64_MAX. `seal()` termine irréversiblement cette allocation. Aucun compte doté de monnaie ne peut ensuite être ajouté. Population vide : sa part initiale reste au Trésor. Les reliquats citoyens/entreprises sont distribués unité par unité aux premiers comptes : la somme est exactement `money_supply`.

## Transferts et atomicité

Un transfert ordinaire débite A de n et crédite B de n. Sont refusés : n≤0, compte absent, A=B, solde insuffisant, crédit débordant. Le refus conserve les deux soldes et possède sa propre ligne datée dans le journal. L'entrée mémoire est allouée avant modification des soldes; si l'allocation échoue, les soldes restent intacts. Après validation, les deux opérations entières ne peuvent pas lever d'exception. La garantie est celle d'un processus mono-thread, pas d'une base de données transactionnelle résistante aux coupures électriques.

Les montants cumulés sont distincts de M(t). Les coûts, chiffres d'affaires, salaires et production n'entrent jamais dans le calcul de la masse. Les destructions d'actifs ne touchent pas les comptes. Les achats et prestations refusés ne livrent pas de ressource. Les factures exceptionnelles restent dues; les salaires refusés sont observables mais les arriérés contractuels ne sont pas encore des passifs persistants.

## Arrondis et limites

Les taux monétaires utilisent des points de base (0 à 10000). `fraction(n,b)` = `(n/10000)*b + ((n%10000)*b)/10000` : arrondi inférieur, sans multiplication intermédiaire risquée. Le reliquat d'une taxe reste chez le payeur. Les soldes et transferts n'utilisent jamais double. Les doubles de statistiques et l'ancien `getSavings()` sont uniquement des vues; ils ne servent pas de montant de transaction. La somme des soldes contrôle à chaque addition les signes et débordements. Les prix configurés sont bornés pour permettre leur multiplication par dix.

## Journal et politique monétaire

Chaque ligne contient id monotone, jour, source, destination, montant, TransactionType et TransactionStatus. Sans archive : toutes les lignes sont conservées en RAM. Avec `archiveTo`/`--ledger-log` : toutes les lignes sont écrites en CSV, les blocs mémoire anciens sont libérés tous les 100000 enregistrements. L'archive est ouverte avant tout transfert et vérifiée/vidée chaque jour; les erreurs d'écriture arrêtent la simulation. Les enums CSV sont les valeurs ordinales des headers TransactionType.hpp; la source ne doit pas être réordonnée sans migration du format.

`MonetaryCreation`/`MonetaryDestruction` sont des types réservés; `transfer()` les refuse par exception. L'opération privée `monetaryOperation()` lève toujours une erreur en V1. Il n'existe donc aucune API publique permettant aux moteurs de créer ou détruire de monnaie. Un futur contrôleur monétaire nécessitera une autorité explicite, un journal dédié et une référence M0 ajustée; ce mécanisme reste désactivé.

## Vérifications

LedgerTest : transferts valides/refusés, comptes inexistants, auto-transfert, limites INT64, arrondis et 100000 transferts reproductibles. CompanyTest/ConsumptionSystemTest : paiements contre livraison, stocks. IncomeSystemTest : rémunérations, allocations et entreprise sans liquidités. TaxationSystemTest : taxe et absence de double prélèvement. CrimeSystemTest : vol, restitution et amende. ExceptionalPaymentTest : facture persistante sans fonds. DisasterSystemTest : dégâts matériels sans variation monétaire. ArchiveTest : 100005 lignes et rotation mémoire. SimulationIntegrationTest : invariants quotidiens, plusieurs années, reproduction exacte, population vide et trésorerie nulle. Voir le rapport pour les exécutions réellement réalisées.
