# Modèle économique V1

## Périmètre

Économie fermée : comptes citoyens, six entreprises et Trésor. Pas d'exportations ni de compte étranger. Le pétrole ne rapporte de monnaie qu'après une vente intérieure. Son extraction utilise le travail de salariés et expose l'entreprise aux mêmes contraintes de liquidité que les autres secteurs.

Chaque entreprise a un secteur, un compte, des identifiants salariés, une productivité, une capacité relative [0,1], un stock et un prix en centimes. Les métiers viennent des CSV existants, complétés par `profession-economics.csv`. Les travailleurs se répartissent entre pétrole et industrie; les autres métiers utilisent leur secteur configuré. Il n'y a pas encore de changement d'emploi automatique. Une rémunération refusée est journalisée, mais ne devient pas une créance salariale juridiquement modélisée.

## Production et ventes

Travail efficace = somme(productivité du métier × santé/100 × (1−fatigue/150)). Production = travail efficace × productivité entreprise × capacité × coefficient climatique. La production s'arrête quand le stock atteint 60 jours de production théorique actuelle; aucun stock existant n'est effacé par cette limite. Une sécheresse réduit la production agricole pendant 60 jours.

Un achat nécessite au moins une unité physique et un transfert accepté au prix entier du fournisseur. Après paiement, le stock diminue exactement d'une unité. Aucun revenu n'est ajouté sans vente ou contrat public explicite. Les achats quotidiens portent sur nourriture, logement, énergie, biens de transport et services essentiels. L'ordre du premier acheteur tourne quotidiennement pour limiter un biais d'identifiant en pénurie.

`TradeSystem` organise des ventes intérieures d'énergie à l'industrie et de biens à la construction. L'input acheté restaure 0,001 de capacité, au maximum 1. Ce modèle est simplifié : il n'impose pas encore de nomenclature d'intrants pour chaque unité produite. Le temps de travail et les salaires représentent les coûts d'extraction/production; aucune charge monétaire sans destinataire n'est soustraite.

Les six stocks représentent des unités sectorielles agrégées. Les services et logements ne sont pas des biens durables individuels; leurs stocks ne périment pas encore. `Resource` prépare la nomenclature de ressources multiples, mais le stock courant est mono-produit par secteur.

## Revenus, dépenses et impôts

Salaires privés mensuels = salaire de référence configuré par métier; le salaire public militaire utilise le paramètre général `wage`. Paiement complet ou refus, sans découvert. Les allocations mensuelles répartissent `welfare_rate` points de base du Trésor entre citoyens sans employeur, hors militaires. Le reliquat de cette division reste au Trésor. Les taxes sont prélevées le même jour sur les salaires effectivement versés, avant la consommation, puis le flux taxable est remis à zéro. Les transferts sociaux ne sont pas imposés dans cette V1.

Une panne ou maladie crée une facture persistante (`pendingCost`, fournisseur). Au plus un besoin exceptionnel à la fois; une facture non réglée n'est jamais remise à zéro. Lorsque le stock du prestataire existe et que le compte dispose des fonds, paiement puis livraison. Les soins rétablissent 15 points de santé, plafonnés à 100. Une facture médicale impayée aggrave la santé quotidienne. Les travaux après catastrophe vont au secteur construction. Les factures ne sont pas encore divisibles, assurables ou finançables à crédit.

## Prix et indicateurs

Chaque mois, prix +max(1,prix/50) si stock inférieur à une journée de demande nationale; prix −max(1,prix/50) si stock supérieur à quinze journées. Prix borné entre un dixième et dix fois le prix initial. Cet ajustement de 2% est une hypothèse structurelle, pas un équilibre général estimé. Le prix des services utilise le même signal simplifié.

Indice de prix = somme des six prix / somme des six prix initiaux, panier fixe d'une unité de chaque secteur. Inflation quotidienne = indice courant / indice précédent −1 (zéro les jours sans changement de prix). Production = unités physiques produites ce jour; addition intersectorielle pédagogique, pas PIB monétaire.

Prospérité = 50 × fraction de citoyens ayant acheté leur alimentation + 50 × taux d'emploi de la population active. Elle mesure l'accès alimentaire et l'emploi, ne redistribue jamais de monnaie et ne représente pas une mesure scientifique exhaustive. Bonheur, stabilité et bien-être sont calculés séparément.

Le paramétrage ne garantit pas un équilibre durable. Les entreprises peuvent accumuler ou perdre leurs liquidités, les salaires être refusés et une crise s'auto-entretenir. Aucun renflouement automatique n'est ajouté pour forcer un résultat favorable.
