# Interface SFML native

Environnement détecté : SFML **2.6.1**, `pkg-config`, police DejaVu Sans, affichage WSLg `:0` avec serveur X Microsoft répondant à `xdpyinfo`. L'interface utilise l'API SFML 2.x, sans migration.

```bash
make                    # CLI et moteurs, aucune dépendance SFML
make test
make sfml               # second exécutable
make run-sfml
./build/dictatorsim-sfml --width 1920 --height 1080
./build/dictatorsim-sfml --population 10000 --width 1280 --height 720
```

Sur une autre installation Ubuntu :

```bash
sudo apt update
sudo apt install g++ make pkg-config libsfml-dev fonts-dejavu-core
pkg-config --modversion sfml-graphics
echo "$DISPLAY"
```

Le build graphique refuse clairement SFML absent ou version 3.x. Les cibles CLI/test restent indépendantes des fichiers `src/ui/` et des bibliothèques graphiques. WSL doit disposer de WSLg ou d'un serveur graphique fonctionnel; définir simplement DISPLAY ne suffit pas à prouver sa disponibilité.

Options : `--population N`, `--seed N`, `--font FILE`, `--width N`, `--height N`. Défaut fenêtre 1600×900, mise en page logique 1280×720. Le redimensionnement maintient le rapport 16:9 avec bandes latérales éventuelles; les coordonnées de clic sont transformées par la vue. 1280×720 et 1920×1080 sont les résolutions de validation. En dessous, le texte est réduit proportionnellement; ce n'est pas la cible d'accessibilité.

Responsabilités : GameApplication possède fenêtre, boucle et partie; DashboardScreen compose les panneaux; StatisticsPanel affiche snapshots et courbe; DecisionPanel gère catalogue/confirmation/historique; EventPanel présente les faits datés; PauseMenu gère retour, nouvelle partie et sortie; UITheme cache police, textes et formes. Aucun header SFML dans core/economy/politics/society.

La limite de rendu est 60 FPS; la logique utilise le temps écoulé monotone, avec au plus quatre journées par frame et conservation du retard. Les statistiques métier sont mises à jour par journée/action; l'interface ne parcourt pas la population. Une police est chargée une fois. Les textes et formes sont réutilisés par identifiant; les graphes ne sont reconstruits qu'au changement de jour. Le thread reste unique; une journée exceptionnellement coûteuse peut néanmoins retarder une frame.

Validation graphique opt-in :

```bash
./build/dictatorsim-sfml --smoke 120 --width 1280 --height 720
./build/dictatorsim-sfml --benchmark 360 --population 10000
```

`--smoke` ouvre une vraie fenêtre et fournit des positions de clic au code de boutons pour tester temps, ordre fiscal, crise, historique et menu. Il injecte une notification identifiée SMOKE TEST, jamais en fonctionnement normal. Captures dans `build/sfml-60.png`, `sfml-70.png`, `sfml-100.png`, `sfml-smoke.png`. Ce mode exécute volontairement un ordre de test, sans décideur IA. `--benchmark` avance à 10x sans aucune action politique; les crises naturelles continuent à bloquer le temps normalement. Il mesure une exécution sur la machine présente, sans garantie universelle de 60 FPS. Résultats réels dans `docs/validation-v1.1/`.

Limites : UI en anglais, pas de sauvegarde de partie ni de raccourcis configurables; pas de graphiques historiques multiples; état détaillé des détenus/localités agrégé. Pas d'animations décoratives ni de shaders. Nouvelle partie remet à zéro l'historique et remplace l'archive financière graphique.
