# Cycle déterministe

Jour zéro : chargement/validation, génération par la Factory existante, allocation exacte de monnaie, entreprises et emplois, verrouillage du Ledger, premier snapshot.

Chaque `Simulation::step()` :

1. Horloge +1 jour, remise du rapport précédent au fournisseur de décisions, au plus une action validée.
2. Catastrophes : dommages physiques et besoins de reconstruction.
3. Armée : réconciliation des soldats, soldes si fin de mois, loyauté, tentative de coup utilisant l'état politique précédent.
4. Production physique; salaires privés mensuels; allocations; taxes mensuelles sur salaires privés et publics.
5. Échanges intermédiaires; consommation alimentaire; logement/énergie/transport/services; factures exceptionnelles persistantes.
6. Prix en fin de mois, indice et inflation.
7. Corruption; criminalité; inégalité; influence à partir d'un instantané; santé/fatigue/stress/bonheur/confiance/colère.
8. Manifestations et rébellion; légitimité et stabilité.
9. Vidage/vérification de l'archive, collecte des événements, statistiques, invariants.

Les salaires précèdent les achats du jour de paie, les taxes suivent immédiatement les salaires. Les loyautés et événements politiques ne se recalculent pas récursivement après chaque conséquence. Chaque moteur ne s'exécute qu'une fois dans le cycle. La semaine de sept jours et les mois de trente jours sont des simplifications explicites.

Les flux aléatoires sont persistants et séparés par sous-système. Les vecteurs imposent un ordre stable; aucune itération de HashMap ne décide d'un tirage ou d'un transfert. Le même exécutable, les mêmes CSV, la même graine et les mêmes actions produisent les mêmes transactions et événements.
