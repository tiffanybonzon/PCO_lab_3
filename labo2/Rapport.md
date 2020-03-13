# Introduction

**Auteurs : Tiffany Bonzon et Jérôme Arn** 

le but de ce laboratoire est d'implémenter une solution d'attente active pour la programmation concurrente. 

# Approche  du problème 

Nous avons choisit deux différentes approches pour ce problème qui seront détaillées plus précisément plus loin. De plus nous avons à notre des dispositions des barrières mémoires et une fonction permettant de "donner" volontairement la ressource au processeur lorsque le thread ne fait rien (comme dans une boucle d'attente). 

## Lamport 

Après avoir implémenté et testé le pseudo code, nous avons remarqué que si on augmentait le nombre de thread ou le nombre d'itérations  ou même les deux paramètres en même temps cela ralentissait beaucoup l'exécution du programme. 

Même après avoir testé différente variante avec les barrières mémoire, nous n'avons pas obtenu de résultat significativement meilleure. Cependant pour des valeurs de threads jusqu'à 10 et un compteur allant jusqu'à 1000, les résultats se trouvent à 100% de justesse. 

C'est pour cette raison que nous avons implémenté l'algorithme de Peterson pour voire s'il y avait une différence. 

## Peterson

Cette deuxième implémentation s'est révélée bien meilleure car elle supportait une large quantité de threads et un nombre élevé d'itérations. Le seul problème rencontré concerne la justesse des résultats. En effet, le compteur a une plage d'erreur pouvant aller jusqu'à 5%. Cette erreur est indépendante du nombre d'itération ou du nombre de threads.

Là aussi nous avons essayé plusieurs stratégie pour les barrières mémoires mais sans améliorations. 

# Conclusion

Nous avons vu que les implémentations faites ne sont pas parfaites. Ces imperfections sont dues, à notre avis, à une mauvaise implémentation de notre part ou à une mauvaise utilisation des barrières. En conclusion, il est intéressant de voir la différence entre chaque implémentation et de pouvoir comparer leurs performances.  