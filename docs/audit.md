# Audit initial
Les neuf headers, six implémentations, six tests, le CSV, la CLI et le Makefile ont été lus avant modification. Aucun AGENTS.md trouvé dans le projet.
Les six exécutables de tests existants ont réussi avant modification (une recompilation propre sera effectuée après intégration).
Namespace ds, classes PascalCase, méthodes camelCase, headers pragma once. Le vecteur de PopulationManager est l'unique propriétaire des citoyens; citizenIndex indexe ses positions. Suppression swap-and-pop correcte.
Adaptations nécessaires : soldes centralisés en centimes, accès aux états des citoyens sans permettre de modifier leurs identifiants, validation des poids, compilation séparée et dépendances automatiques. Les anciens champs income/savings sont des interfaces historiques à retirer progressivement; aucun moteur ne les utilisera comme comptes.
La Factory et le provider restent dédiés à la génération et au chargement. Les paramètres métier sont chargés séparément pour préserver le CSV existant.

## Adaptations réalisées

Le champ savings du citoyen a finalement été supprimé : getSavings lit maintenant le compte Ledger et reste compatible avec le test de construction. L'identifiant est privé; l'index reste une référence vers le vecteur. La génération construit vecteur et index temporaires avant swap pour préserver la cohérence en cas d'échec d'allocation. ProfessionConfig et CSVProfessionProvider ont reçu des extensions métier additionnelles, sans remplacement du CSV ni des six tests d'origine. Les fichiers de modèles détaillent les limites fonctionnelles restantes.
