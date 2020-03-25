# Introduction

**Auteurs : Tiffany Bonzon et Jérôme Arn** 

# Approche  du problème 

Les tâches de gestion de la barre d'avancement et de répartition des calculs entre les threads ont été attribuées au thread principal. Tandis que la génération de tous les hashs sur un intervalle donné, la comparaison avec le hash a cassé ainsi que l'incrémentation du compteur principal ont été attribués aux threads. La quantité de code relativement importante qui compose les threads s'explique car elle est intrinsèquement liée entre elle. Néanmoins la partie critique modifiée et lue par le thread principal et les threads de génération de hashs est restreinte à une variable.

Cette variable est globale à tous les threads qui génèrent des hashs et elle est protégée par un mutex lors de son incrémentation et lors de sa lecture par le getter. 

Dans un premier temps, nous avions pensé facilité le passage des nombreux arguments en transmettant un objet Threadmanager avec des getter et des setter qui seraient ensuite utilisés dans les threads. Mais après différent tests, nous avons pu remarqué que cela ralentissait considérablement les calculs. Pour cela nous avons transmis tous les arguments en copie. 

# Test du programme

En règle générale, tous nos tests ont menés à un résultat concluant. Mais de manière aléatoire et sans que nous ayons pu en déterminer la raison, deux tests ont finis de manière inattendue. Nous n'avons pas pu reproduire la situation en mode debugg.

## Pour un mot de passe de 3 caractères

On peut voir dans ces exemples que plus on augmente le nombre de threads, plus le temps diminue. 

![](/home/reds/pco20_student/labo3/img/3char1th.png) 

![](/home/reds/pco20_student/labo3/img/3char3th.png)

![](/home/reds/pco20_student/labo3/img/3char10th.png)

## Pour un mot de passe de 4 caractères

On peut voir dans ces exemples que plus on augmente le nombre de threads, plus le temps diminue. 

![](/home/reds/pco20_student/labo3/img/4char1th.png)

![](/home/reds/pco20_student/labo3/img/4char3th.png)

![](/home/reds/pco20_student/labo3/img/4char5th.png)

## Pour un mot de passe de 5 caractères

On peut voir que la recherche avec 5 threads est presque trois fois plus lente que la recherche avec 7 threads. Lors de nos tests, nous avons constaté qu'avec 10 threads cela ne fonctionnait pas pour une raison que nous n'avons pas pu éclaircir durant le temps donné. 

![](/home/reds/pco20_student/labo3/img/5char5th.png)

![](/home/reds/pco20_student/labo3/img/5char7th.png)

# Conclusion

