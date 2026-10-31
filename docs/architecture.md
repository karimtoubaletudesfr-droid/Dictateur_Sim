# Architecture et propriété

`Simulation` possède `World`, `StatisticsEngine` et `DecisionManager`. Il orchestre des composants concrets, sans formules métier. `World` possède un unique `PopulationManager`, les comptes du `Ledger`, les six entreprises, le gouvernement, les états agrégés et cinq flux aléatoires métier indépendants.

`PopulationManager` conserve `std::vector<Citizen> population` et `std::unordered_map<std::uint32_t,std::size_t> citizenIndex`. La recherche mutable ajoutée donne accès à un citoyen existant; son identifiant est privé. Le vecteur n'est jamais fourni en accès mutable. La suppression swap-and-pop et les tests existants sont conservés. La démographie en cours de simulation est volontairement absente; appeler directement une suppression nécessiterait de réconcilier employeurs/comptes et soldats. Les comptes d'un citoyen disparu ne peuvent pas être supprimés silencieusement.

Chaque citoyen référence un compte; il ne possède pas de solde. `getSavings()` est conservé pour compatibilité et lit le Ledger (conversion en unités pour cet ancien getter); aucune décision comptable ne passe par cette conversion double. `income` est le dernier flux salarial observé, pas un stock. Le pointeur non propriétaire vers Ledger exige que World vive plus longtemps que les citoyens; World n'est pas copiable.

Le Ledger détient des Account, dont le solde est privé. Seul Ledger est ami; aucun moteur ne possède d'accesseur mutable. Treasury est une façade sur l'identifiant du compte public, pas un second compte. Les entreprises et l'armée référencent des identifiants citoyens, jamais des copies.

Les systèmes ont `update(World&)` afin de conserver des interfaces simples dans cette V1. Cela offre un contexte large mais aucune dépendance circulaire entre moteurs : les headers forward-déclarent World; les includes complets se trouvent dans les `.cpp`. Une granularité plus stricte des vues pourra être ajoutée si plusieurs équipes développent des moteurs concurrents.

EventBus est une file de faits FIFO, vidée une fois par jour dans EventLogger. Il n'exécute pas de callbacks et ne peut pas générer une boucle d'abonnés. Les conséquences physiques et économiques sont appliquées par les systèmes concernés; les événements les expliquent. Ce n'est pas un système de rejeu complet ni une architecture entièrement pilotée par événements.

Configuration validée avant exécution; invariants dynamiques vérifiés chaque jour. Les systèmes lisent des valeurs du jour précédent lorsque l'ordre l'impose (notamment loyauté et coups). RNG population = seed; économie, société, armée, catastrophes et politique utilisent des seeds dérivées par XOR avec des constantes distinctes. Cela isole les séquences de tirage entre sous-systèmes, sans prétendre prouver leur indépendance statistique.

Le rapport exact est une valeur copiée, distincte de World. Les décisions peuvent être proposées sans accès à l'état mutable. `poll()` ne bloque pas et traite au plus une action par jour; les identifiants futurs ou déjà traités sont rejetés. Les décisions plus anciennes que la dernière consommée ne sont pas rejouées.
