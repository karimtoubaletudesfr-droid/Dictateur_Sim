# Gravité des événements

`EventClassifier` transforme les faits existants en `PlayerNotification`, sans retirer le journal V1. `DecisionRequest` est étendu avec texte et choix structurés, sans créer une chaîne concurrente de décisions.

| Gravité | Déclencheurs V1.1 | Affichage / comportement |
|---|---|---|
| Minor | Prix, paiements, incident individuel | Journal; aucune pause |
| Important | Début de manifestation | Journal marqué; pas de réponse imposée |
| Major | Catastrophe, révélation d'action secrète, mobilisation >10% des citoyens | Briefing et pause; rappel par catégorie après 30 jours |
| Critical | Menace de coup, rébellion, effondrement | Pause obligatoire et choix, ou fin de mandat |

Catégories : économie, société, politique, armée, catastrophe. Les actes individuels de criminalité et maladie restent mineurs pour éviter une interruption par citoyen. La classification des catastrophes ne distingue pas encore leur sévérité physique : même un dommage modéré peut produire un briefing. Les tendances chômage/production n'ont pas encore leurs propres alertes automatiques.

Les notifications ne changent aucun solde. Les effets matériels viennent toujours des moteurs. Les événements informatifs ne sont pas des ordres; seule une réponse du joueur entre dans `PlayerDecisionController`. Les réponses refusées maintiennent la fenêtre ouverte. Après réponse acceptée ou « Take no action », la simulation reste en pause. Une nouvelle crise critique en attente est présentée immédiatement, sans avancer le temps.

Tests : EventSeverityTest (quatre classes), PlayerSessionTest (pause, délais, file de crises simultanées), PlayerCoupTest (résultat militaire différé), scénario graphique `--smoke` (modal réelle et réponse sans action).
