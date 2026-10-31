# Modèle social

Les paramètres sont fictifs, explicables et non diagnostiques. Une profession ne détermine ni maladie mentale ni criminalité.

Alimentation : +20 si achat livré, −25 sinon; logement : +10 si service payé, −10 sinon, bornes [0,100]. Fatigue : charge physique métier × coefficient configuré × heures/8, moins repos; chaque septième jour est un jour de repos. Stress : +1 par facture en attente, sinon −0,5; ajout 0,2 × charge mentale en jour travaillé, sinon −1. Santé : −0,5 si alimentation<30, sinon +0,1; −0,2 supplémentaire pour soins impayés. Pas de décès automatique à santé zéro en V1.

Bonheur cible = moyenne pondérée configurable d'alimentation, logement, santé, (100−fatigue), moins 0,15×stress. Bonheur suivant = 0,9×ancien + 0,1×cible, borné. Les coefficients des besoins sont chargés depuis society.csv; les inerties et variations unitaires sont pour l'instant des constantes structurelles documentées.

Confiance suivante = confiance +0,01×(bonheur−50) −0,15×Gini −0,1 en état d'urgence. Influence : chaque citoyen reçoit 2% de la confiance de son voisin dans un anneau déterministe, à partir d'une copie des valeurs précédentes. Ce réseau n'est pas un réseau social géographique.

Colère suivante = 0,95×ancienne +0,05×[0,45×(100−confiance)+0,30×(100−alimentation)+25×Gini]. Deux citoyens de bonheur égal peuvent avoir des confiances et colères différentes. Gini calculé sur les soldes citoyens triés : somme((2i+1)×solde_i)/(n×somme soldes)−1, indices depuis zéro. Zéro si tous les soldes sont nuls ou si la population est vide.

Probabilité quotidienne d'entrer en manifestation = taux configuré × colère/100 × (1−confiance/100) × (0,5+fraction manifestants précédente) × risque. Risque=0,25 en urgence, 0,7 sinon. Sortie quotidienne = 0,03+0,1×confiance/100. La rébellion dépend simultanément de mobilisation, colère et illégitimité; son issue peut rétablir le régime, déclencher une transition ou un effondrement. Aucune date n'est prédéterminée.

Tentative de vol : taux configuré × (0,1+0,5×privation alimentaire+0,4×tolérance individuelle au risque) × (1−0,5×police). Une personne aisée peut donc voler, une personne pauvre peut ne jamais le faire. Victime distincte tirée uniformément; succès et signalement séparés. Montant = minimum(coût réparation configuré, solde victime). Un succès fait une transaction Theft. Si signalé, restitution immédiatement tentée puis amende jusqu'au solde disponible. Chaque étape est explicite; ce modèle ne représente pas une procédure judiciaire réaliste. Les tentatives signalées incluent les échecs, contrairement au compteur de vols réussis : les deux compteurs n'ont pas le même dénominateur.

Bien-être national = moitié bonheur moyen, moitié santé moyenne. Criminalité = vols réussis cumulés pour 1000 citoyens et par jour écoulé. Les événements rapportent des mécanismes directement appliqués; les agrégats ne prouvent pas de causalité statistique. SocialGroup et CrimeProfile restent des contrats de données préparatoires.
