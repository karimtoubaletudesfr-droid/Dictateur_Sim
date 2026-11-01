# DictatorSim V1.1 — le joueur gouverne

Simulation pédagogique d'un pays fictif : citoyens individuels, entreprises, paiements, besoins, opinions, contestation, gouvernement, armée et catastrophes. Les résultats proviennent de règles explicites, de stocks et de transferts; la prospérité ne crée jamais de monnaie. Ce modèle n'est pas une prévision scientifique des comportements humains.

## Jouer avec SFML

Le joueur prend toutes les décisions gouvernementales. Aucun décideur IA, aucun LLM, aucun appel réseau. La société continue à vivre automatiquement.

```bash
make sfml
make run-sfml
# Ou :
./build/dictatorsim-sfml --width 1920 --height 1080
```

La partie commence en pause. Boutons PAUSE/1x/2x/5x/10x, Espace pour pause/reprise, Échap pour le menu. GOVERN → sélection → REVIEW & APPLY → confirmation. La molette parcourt les listes. Les crises bloquent le temps jusqu’à une réponse, puis il faut reprendre manuellement. MENU permet une nouvelle partie ou la fermeture. Un remplacement du dirigeant ou un effondrement termine le mandat.

SFML 2.6.1 et WSLg ont été détectés sur cette machine. Sur une autre installation : `sudo apt install libsfml-dev pkg-config fonts-dejavu-core`, avec un affichage WSLg fonctionnel. L’application charge DejaVu Sans; `--font FILE` permet une autre police. Le CLI et les tests logiques restent compilables sans SFML.

Documentation V1.1 : [mode joueur](docs/player-mode.md), [inventaire des 23 décisions/réponses](docs/political-actions.md), [interface SFML](docs/sfml-interface.md), [gravité des événements](docs/event-severity.md), [conséquences et historique](docs/decision-impact.md). Tests supplémentaires : `make test-player`. Résultats réels : [rapport V1.1](docs/test-report-v1.1.md).

Limites de cette livraison : UI en anglais, aucune sauvegarde/reprise, localités et détenus agrégés, interventions publiques simplifiées. Les actions autoritaires sont uniquement des représentations abstraites de fiction, avec coûts, victimes, pertes de confiance et risques de radicalisation; aucun détail opérationnel.

## Compiler et exécuter

Sous Ubuntu/WSL, avec G++ et GNU Make (C++17, bibliothèque standard seulement) :

```bash
cd /home/touba/DictateurSim/Dictateur_Sim
make clean
make -j4
make test
make run
./build/dictatorsim --population 200 --days 3600 --seed 42
./build/dictatorsim --population 10000 --days 3650 --seed 42 --ledger-log build/transactions.csv
```

`--population`, `--days`, `--seed`, `--tax-bps` remplacent les paramètres chargés. `--config DIRECTORY` sélectionne le répertoire contenant tous les CSV. Les chemins sont relatifs au répertoire courant. `--help` liste les options. Une erreur de configuration termine avec un code non nul.

`--ledger-log FILE` écrit **toutes** les transactions dans un CSV, y compris les refus; seul un bloc récent de 100 000 entrées reste en mémoire. Le fichier est remplacé au lancement. Sans cette option, l'historique complet reste en mémoire : pour une grande population sur plusieurs années, utiliser l'archive. Les événements et statistiques restent en mémoire. Aucun fichier de sauvegarde/reprise de World n'est encore fourni.

Le calendrier simplifié compte 360 jours par an, douze mois de trente jours. Au jour 3600, dix années sont écoulées et l'année courante est 11. Les salaires et impôts sont mensuels, jamais quotidiens.

## Architecture

| Module | Responsabilité |
|---|---|
| core | Configuration, horloge, état du monde, orchestration déterministe |
| population / providers / tools | Génération existante, professions, identifiants, vecteur et index |
| economy | Comptes centralisés, entreprises, production, ventes, salaires, impôts, besoins exceptionnels |
| society | Besoins, bonheur, confiance, influence, criminalité, manifestations, rébellion |
| politics | Validation des actions, dépenses publiques, légitimité et stabilité |
| military | Identifiants des soldats, soldes publiques, loyauté et coups d'État |
| disasters | Risques régionaux, dommages physiques, sécheresse temporaire |
| decisions | Propositions structurées et file non bloquante; aucun accès aux soldes |
| events / statistics | Faits datés, historique, agrégats et rapport exact |

Headers dans `include/dictatorsim`, implémentations dans `src`, CLI dans `apps/cli/main.cpp`. Compilation séparée `.o`, dépendances `-MMD -MP`, un seul `main` par exécutable. SFML est limité au second exécutable graphique. Aucun réseau, LLM, Singleton ou CMake.

## Tests

```bash
make test-economy
make test-society
make test-politics
make test-military
make test-disasters
make test-simulation
```

Les anciennes commandes `test-id`, `test-citizen`, `test-random`, `test-provider`, `test-factory`, `test-population` restent disponibles. `make test` découvre tous les `tests/*Test.cpp`, y compris les tests additionnels de configuration, décisions et archive. Les résultats réellement exécutés figurent dans [test-report](docs/test-report.md).

## Configuration

Les neuf fichiers de paramètres généraux utilisent `key,value` avec commentaires `#`. Paramètres inconnus, doublons, valeurs non finies et bornes invalides sont refusés. Les valeurs absentes conservent les défauts de `SimulationConfig`; les fichiers eux-mêmes sont obligatoires.

`professions.csv` conserve son format d'origine. `profession-economics.csv` ajoute salaire mensuel en centimes, secteur (0 agriculture, 1 pétrole, 2 industrie, 3 construction, 4 santé, 5 commerce), productivité relative, charges physique/mentale, heures et compétence. Le provider fusionne les données, la Factory ne calcule aucun revenu. La compétence est une donnée préparatoire; elle n'impose pas encore de recrutement.

`money_supply` fixe exactement la monnaie initiale. `citizen_share` et `company_share` sont des points de base, le reste va au Trésor. Les reliquats entiers sont distribués sans perte. Les prix/coûts sont en centimes. Les probabilités sont quotidiennes. `risk_R_H` précise le risque relatif par région et type de catastrophe. Voir les documents des modèles pour les unités et règles.

## Fonctionnalités opérationnelles et simplifications

La boucle CLI produit des valeurs calculées : monnaie, Trésor, chômage, production physique, prix, bonheur, colère, santé, criminalité, protestataires, stabilité, loyauté et bilan comptable. Une exécution peut se terminer en crise tout en restant comptablement cohérente. `CONSISTENT` signifie que les invariants ont été vérifiés, pas que le pays est prospère.

Les transferts, refus, vols, restitutions, amendes, dépenses et allocations publiques sont réels. Les besoins non satisfaits affectent les citoyens. Les catastrophes détruisent des ressources, pas de monnaie. Les décisions `Simulation::act` sont validées, et la file de décisions permet une future interface.

Les modèles secondaires sont volontairement **PARTIAL** : six entreprises agrégées, affectation initiale des métiers, pas de marché du travail dynamique, pas de mortalité/naissances/migration, pas de prêts ni de faillite juridique. Santé et criminalité ne constituent pas des modèles médicaux ou sociologiques. Les interventions d'éducation/santé/police utilisent des contrats agrégés; les infrastructures ne sont pas des bâtiments individuels. Le commerce extérieur est désactivé. Les stocks de service utilisent la même représentation simplifiée que les biens et ne périment pas. Le régime peut changer de statut, mais aucune constitution ou succession détaillée n'est simulée.

Les anciens contrats de fournisseurs sont conservés mais **désactivés en mode joueur**; les modes autonome et hybride sont hors périmètre V1.1. La personnalité du dirigeant est validée et configurable en C++, sans multiplicateurs universels. Les coefficients structurels secondaires sont documentés mais pas tous externalisés. Tous les écarts et états sont détaillés dans [l'inventaire](docs/class-inventory.md).

## Comprendre et prolonger

- [Audit et adaptations](docs/audit.md)
- [Architecture](docs/architecture.md), [cycle temporel](docs/simulation-cycle.md)
- [Économie](docs/economy-model.md), [invariants comptables](docs/accounting-invariants.md)
- [Société](docs/society-model.md), [politique](docs/politics-model.md), [armée](docs/military-model.md), [catastrophes](docs/disaster-model.md)
- [Inventaire exhaustif](docs/class-inventory.md), [validation](docs/test-report.md), [fichiers modifiés](docs/change-summary.md)

L’interface SFML utilise `PlayerSession`, `Simulation::step()` et les rapports existants. Elle soumet exclusivement des actions structurées au contrôleur joueur; aucun accès mutable à World ou Ledger. La boucle et les commandes restent sur le thread propriétaire.

Les prochaines versions pourront introduire ménage/solidarité, recrutements et faillites, ressources intermédiaires obligatoires, démographie, régions détaillées, reprise de sauvegarde, rapports imparfaits et contrôleur IA. La reproductibilité est garantie dans le même environnement de compilation et de bibliothèque standard; les distributions standard ne garantissent pas une séquence identique entre toutes les implémentations C++.


### Parametres interactifs et carte urbaine

Dans SFML, **P** ou **MENU > PARAMETRES DU PAYS** ouvre les reglages et
met le temps en pause. Les onglets proposent les salaires des huit metiers
remuneres (y compris les soldats), les probabilites de base, les seuils de
rebellion/putsch et les couts. Chaque ligne explique son effet et son unite.
Cliquez une valeur, utilisez Ctrl+A pour la remplacer, puis Entree, ou les
boutons -/+. **APPLIQUER A LA PARTIE** valide tout le formulaire sans changer
les comptes ni le jour. Les salaires prennent effet a la prochaine paie,
sous reserve des fonds disponibles. Les citoyens sans emploi, etudiants et
retraites dependent des aides existantes, pas d'un salaire d'employeur.

L'onglet **Population** accepte de 1 a 1 000 000 citoyens. **CREER UN PAYS**,
puis la confirmation, reinitialise la partie avec les reglages affiches.
Les grandes populations augmentent le temps de calcul. Les reglages restent
en memoire pendant la session ; ils ne remplacent pas les fichiers CSV.
Retour abandonne les modifications non appliquees.

**CARTE / BUREAU** alterne entre le tableau de bord et une carte urbaine en
vue du ciel. Le decor utilise une geometrie SFML statique sans texture externe.
Les zones rouges indiquent les manifestations, rebellions, menaces de putsch
et catastrophes des dix derniers jours. Les reperes sont
schematiques ; les catastrophes respectent leur region simulee.

Verification : `make sfml test`, puis `./build/dictatorsim-sfml --smoke 150`.
Ce parcours capture aussi la carte et les onglets de parametrage. Utilisez 230 images pour inclure le nouveau bureau et les formules.


### Le cabinet presidentiel

L'accueil SFML est un bureau illustre : journal du jour, dossier interactif,
television DS 24 (dernier evenement reel) et indicateurs de moral. Les onglets
du bas donnent acces au Bureau, a la Carte, aux Dossiers de decisions et aux
Formules. Le bureau ne modifie pas les regles de simulation.

La carte affiche par defaut 400 petits visages. Les boutons NOMBRE DE VISAGES
permettent de choisir 48, 100, 200, 300, 400, 600 ou 800 citoyens echantillonnes
regulierement, sans depasser la population reelle. Le choix change seulement
l'affichage, et reste actif en changeant de page. Taille et repartition
s'adaptent automatiquement a la densite, en laissant la riviere libre. Les visages dessines dans SFML ne dependent pas d'une police
emoji ; un petit atlas est genere une seule fois puis reutilise. Priorite visuelle : faim (repas < 35), fatigue (>= 65), colere (>= 60),
content (bonheur >= 60), puis neutre. Un cercle rouge signale un manifestant.
Survoler un visage affiche ses valeurs exactes ; les positions sont decoratives.

La page Formules presente 18 fiches en six categories, avec equations,
explications, conditions, unites et fichiers sources du moteur. Elle distingue
les risques de base des probabilites conditionnelles et les regles d'affichage
des equations de la simulation. Verification graphique : `--smoke 230` capture
le bureau, les visages, les reglages et les six categories de formules.


### Audio

SFML 2.6.x (module audio inclus) lit directement `assets/sounds/background.mp3`
en boucle et `assets/sounds/button-press.mp3` pour les boutons actifs. La musique
est diffusee en streaming ; le son court est charge une seule fois en memoire.
Ouvrez **P > Audio** ou **Menu > Parametres > Audio** : les deux jauges vont de
0 a 100 %, par clic ou glissement. Reglages immediats, independants, conserves
pendant la session (y compris une nouvelle partie). Valeurs initiales : musique
30 %, boutons 60 %. Le silence est obtenu a 0 %. Les fichiers manquants sont
signales sans bloquer le jeu. La CLI et les tests du moteur restent sans audio.
Verification graphique et audio : `./build/dictatorsim-sfml --smoke 260`.
