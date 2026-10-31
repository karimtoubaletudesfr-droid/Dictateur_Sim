# Catastrophes

Trois régions fictives : plaine côtière (0), intérieur agricole (1), relief volcanique (2). Appartenance citoyen = id modulo 3; entreprise = id modulo 3. Ce découpage est reproductible mais ne représente pas une géographie réaliste.

Chaque couple région/type reçoit un tirage quotidien de probabilité `disaster_probability × risk_R_H`. H : 0 séisme, 1 tsunami, 2 sécheresse, 3 inondation, 4 éruption. Les risques sont configurables [0,1]; les régions intérieures ont un risque tsunami nul par défaut. Sévérité tirée entre 0,1 et 0,6. Plusieurs événements peuvent coexister le même jour. Les types Earthquake/Tsunami/Drought/Flood/VolcanicEruption sont des contrats de données; DisasterSystem exécute le comportement commun.

Séisme, tsunami, crue et éruption : capacité et stock des entreprises régionales multipliés par (1−sévérité); logement citoyen −60×sévérité, santé −20×sévérité; facture de reconstruction créée si aucune facture n'est déjà en attente. Aucun solde monétaire n'est touché. Les factures ultérieures sont traitées par LivingCostSystem et réglées via Ledger au prestataire.

Sécheresse : coefficient agricole national temporaire = 1−sévérité pendant 60 jours, sans destruction de logement. Malgré son origine régionale, ce coefficient s'applique à l'entreprise agricole unique : limite explicite de l'agrégation sectorielle. Une nouvelle sécheresse remplace le coefficient et l'échéance précédents; les sécheresses ne se cumulent pas encore par région.

Les dégâts n'entraînent pas de création monétaire. Les aides publiques redistribuent des fonds existants. Les bâtiments ne sont pas individualisés, il n'y a ni mortalité ni évacuation. Le test dédié force des dommages physiques et vérifie que comptes et journal monétaire restent inchangés. L'intégration vérifie que modifier les tirages sociaux ne change pas la séquence naturelle.
