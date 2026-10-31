# Conséquences et historique

Pipeline : UI → PlayerSession / PlayerDecisionController → ActionValidator → Simulation::act → PolicyExecutor → World / Statistics. Le rendu ne reçoit qu'une référence const à Simulation et des snapshots; il ne peut pas écrire dans les comptes ou les citoyens.

Chaque soumission enregistre date, PoliticalAction (type, montant en centimes, points de base, cible et visibilité), acceptation/refus, motif, coût immédiat réellement constaté au Trésor, snapshots avant/après. La validation est effectuée avant paiement et revérifiée dans PolicyExecutor. Les événements sont collectés et légitimité/stabilité/snapshot sont rafraîchis après une action acceptée, sans avancer artificiellement d'une journée.

La première observation à partir de J+30 est conservée séparément. Elle est une observation des conséquences possibles, pas une preuve d'attribution : catastrophes, consommation, prix et autres décisions peuvent aussi expliquer la variation. L'historique UI présente coût, paramètres, trésorerie, stabilité, bonheur et observation différée. Les données complètes (santé, criminalité, etc.) restent consultables dans DecisionHistory.

La fiscalité change l'assiette au prochain jour de paie, pas les soldes le jour du vote. Le financement sanitaire paie réellement le secteur santé, ajoute un stock de traitements et augmente sa productivité; les besoins médicaux existants sont ensuite satisfaits si les citoyens peuvent payer. Ce financement ne constitue donc pas encore une assurance maladie universelle. Les reconstructions créent/conservent une facture jusqu'au règlement au constructeur; une prestation livrée restaure du logement.

## Actions autoritaires abstraites

L'opposante Mara Voss et Civic Assembly sont fictifs. Mara est un personnage politique agrégé extérieur au tableau de citoyens, pas un citoyen dupliqué. L'assassinat élimine ce personnage et ajoute une victime; aucun détail opérationnel n'est représenté. La répression et la destruction d'une localité blessent les citoyens concernés (−20 santé, +30 stress); les pertes individuelles ne provoquent pas encore de décès démographiques. Le compteur « victims » additionne les atteintes : une personne atteinte plusieurs fois peut y apparaître plusieurs fois. Les détenus, hors le statut de Mara, sont également un agrégat.

Sévérité politique : censure/interdiction 5, arrestation 10, répression 20, assassinat 25, destruction 35. Peur augmente de cette sévérité, loyauté militaire perd un quart de celle-ci. Un acte public réduit la confiance individuelle de sa sévérité, augmente la colère de sa moitié et la radicalisation de sa sévérité. La peur réduit la propension à manifester; la radicalisation l'augmente. La stabilité résulte ensuite du système existant, sans bonus automatique.

Un acte covert n'applique pas immédiatement la réaction publique globale, mais ses coûts, victimes et effets physiques sont immédiats. Sa révélation quotidienne après le jour de l'acte a une probabilité `min(1, 0.003 × (1 + sévérité/10) × (1 − 0.5 × police))`, utilisant le RNG politique existant. Lorsqu'il est révélé une seule fois : scandale, perte de confiance, hausse de colère/radicalisation, briefing majeur. Censure et interdiction ont aussi leurs effets quotidiens directs sur la confiance. La peur baisse de 0,05/jour, la radicalisation de 0,01/jour. Ce modèle est abstrait et ne représente aucune technique réelle de surveillance ou de dissimulation.

Les coûts vont au prestataire public de services existant; aucun argent n'est détruit. Les dégâts de localité réduisent de moitié stocks/capacités des entreprises régionales, indépendamment de leurs comptes.

Tests : PlayerDecisionImpactTest, PlayerActionsTest, PlayerCoupTest, PlayerSessionTest, plus tous les tests comptables V1. Le test du catalogue vérifie faisabilité et conservation pour les 22 actions; les tests ciblés vérifient en plus fiscalité, soins effectifs, dégâts, victimes, révélation et observation à trente jours. Toutes les formules secondaires n'ont pas de calibration empirique.
