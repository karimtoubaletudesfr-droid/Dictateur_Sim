# Mode joueur V1.1

Le joueur est l'unique autorité gouvernementale. Aucun fournisseur de décisions n'est attaché; `Simulation::setDecisionProvider` refuse tout fournisseur en `playerMode`. Les anciens contrats V1 sont conservés pour compatibilité, sans utilisation dans cette application. Aucun appel réseau, Ollama ou LLM.

## Contrôles

- Nouvelle partie : Menu → NEW GAME. Le pays et les historiques sont réinitialisés avec la configuration/graine du lancement.
- La partie commence en PAUSE. Cliquer 1x, 2x, 5x ou 10x pour avancer. Espace alterne pause/1x hors fenêtres bloquantes.
- À 1x, une journée dure 0,5 seconde réelle. Le rendu ne détermine pas le nombre de journées : l'accumulateur conserve le retard, avec au plus quatre journées par frame.
- Échap ouvre/ferme le menu en restant en pause. Fermer un menu, confirmer une politique ou répondre à une crise ne reprend jamais automatiquement le temps.
- Sélectionner une action dans GOVERN, modifier son allocation si disponible, choisir PUBLIC/COVERT pour les trois actions compatibles, puis REVIEW & APPLY et CONFIRM ORDER.
- Une action impossible est refusée avec explication, sans prélèvement. Les refus restent dans l'historique.
- La molette parcourt les actions, les décisions passées ou les dépêches selon le panneau survolé.

Les politiques restent applicables en pause. La confirmation suspend le temps. Un briefing majeur propose des réponses contextualisées et toujours la possibilité explicite de ne pas agir. Les crises simultanées sont présentées dans l'ordre; un délai de trente jours par catégorie limite les interruptions répétitives non critiques. Une crise critique n'est pas masquée par ce délai.

Un coup d'État en mode joueur produit d'abord une menace; son résultat est différé jusqu'à une réponse puis la reprise du temps. Remplacement du dirigeant, transition ou effondrement termine le mandat : le temps est bloqué, seules nouvelle partie et fermeture restent possibles. Les conséquences politiques d'une crise sont des mécanismes de la société, jamais des décisions gouvernementales automatiques.

Le travail, les dépenses ordinaires, les soldes, les taxes déjà votées, la production et les aléas restent automatiques. Les gouvernements n'augmentent pas spontanément les taxes ou les budgets.

La partie n'est pas sauvegardée. Le journal financier graphique est `build/player-transactions.csv`, réinitialisé à chaque nouvelle partie; il limite l'historique RAM du Ledger à un bloc de 100000 transactions. Les décisions/observations restent en mémoire. UI en anglais pour cette livraison; documentation en français.
