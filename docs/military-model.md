# Armée

Les soldats restent dans PopulationManager. MilitaryUnit contient seulement leurs identifiants; RecruitmentSystem réconcilie cette liste avec les professions existantes. Il ne recrute pas encore de nouveaux soldats : ce composant est PARTIAL.

La limite mensuelle initiale est une fraction configurable de la trésorerie initiale. Chaque mois, une solde complète est transférée du Trésor au soldat si la limite restante et les fonds suffisent. Les refus sont journalisés. `spent` est le flux mensuel, jamais un compte séparé. Les taxes sur ces soldes sont prélevées par TaxationSystem dans le même cycle.

Satisfaction salariale = 100×nombre payé/effectif. Conditions = bonheur moyen des soldats. Loyauté cible = 0,5×satisfaction salariale +0,25×conditions +0,25×légitimité précédente. Loyauté = 0,98×ancienne+0,02×cible. Aucune armée : loyauté/satisfaction zéro, aucune possibilité de coup.

Risque quotidien de coup = coefficient configuré × (1−loyauté/100) × (1−légitimité/100) × (1−stabilité/100). Une tentative peut remplacer le dirigeant (40%), créer une crise prolongée (30%) ou maintenir le régime (30%). Ces probabilités d'issue sont des hypothèses structurelles fixes de V1, pas des fréquences historiques.

Pas de combat tactique, équipement, chaînes de commandement, retraite ni recrutements automatiques. La loyauté est un état agrégé; la santé et les besoins des soldats restent individuels. Les tests vérifient les transferts et références civiles; les issues probabilistes sont surtout couvertes par les simulations reproductibles et nécessitent davantage d'analyse statistique.
