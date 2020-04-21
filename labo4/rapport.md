# Laboratoire 4 - station de ski

Auteurs: Jérôme Arn, Tiffany Bonzon

## Description des fonctionnalités du logiciel

Le but de ce programme est de modéliser une station de ski avec des skieurs et un télécabine. Ces derniers sont représentés par des threads. 

Le but est de pouvoir charger des skieurs dans le télécabine en respectant sa capacité, et ceci quelque soit le nombre de skieurs qui attendent. Le but est de synchroniser le chargement et déchargement des skieur afin qu'ils ne sortent pas en cours de route ou qu'ils ne montent pas dans une cabine en déplacement. 

Comme décrit dans la donnée, il y a N skieurs et une cabine ayant une capacité M. La cabine effectue des trajets entre la station et le sommet de la montagne. 

La cabine monte les skieurs attendant à la station lors de son arrivée, si un skieur arrive à la station alors qu'elle y est déjà, ce skieur devra attendre son retour. S'il y a trop de skieurs attendant par rapport à la capacité de la cabine, alors certains devront attendre le prochain voyage. La cabine peut faire une montée à vide si aucun skieur n'attend à la station. Tous les skieurs descendent de la cabine une fois au sommet. La cabine ne redescend qu'à vide.

Une fois les skieurs en haut de la montagne, ils redescendent en skiant, sans attente.

## Choix d'implémentation

### Méthode run() CableCarBehavior 

Pendant que le télécabine est en service, on effectue en boucle : un chargement, une ascension, un déchargement et une descente. Quand la station est fermée, le thread quitte la boucle et se termine. Si des skieurs sont dans la cabine montante, ils sont déchargés en haut de la montagne (cas géré par `PcoCableCar` grâce au sleep pendant la montée). 

### Méthode run() SkierBehavior

Pendant que le télécabine est en service, on effectue en boucle : l'attente de la cabine, on rentre dedans, on attend que l'ascension soit terminée, on en sort, et on skie. 

Quand la station est fermée:

- Si on se trouve en haut de la montagne, on skie et une fois en bas on termine le thread
- Si on est en attente de la cabine, on termine le thread
- Si on est dans la cabine montante, on attend d'être arrivé en haut de la montagne, on skie jusqu'en bas, et on termine le thread (cas pas géré par le `run()` de skieur, mais par `PcoCableCar`)

### Gestion de l'attente

L'attente se situe au niveau de `waitForCableCar()` et `waitInsideCableCar()`, grâce à deux sémaphores initialisés à 0.

`waitForCableCar()`

- Quand un skieur arrive en bas de la station, il effectue un `acquire()` sur le sémaphore waitForCar
- Ce sémaphore étant déjà à 0, le thread du skieur se bloque
- Les threads sont débloqués au fur et à mesure que les skieurs sont chargés dans la cabine (`loadSkiers()`) grâce à un release sur le sémaphore
- Ceci pour s'assurer que les skieurs n'entrent pas dans une cabine non présente, ou étant remplie à sa capacité maximum

`waitInsideCableCar()`

- Un skieur dans la cabine doit attendre qu'elle atteigne le sommet avant de pouvoir être déchargé

- Le fonctionnement est le même que pour `waitForCableCar()` : Un skieur attendant dans la cabine effectue un `acquire()` sur waitInsideCar qui est à 0 et donc bloque le thread, puis la méthode `unloadSkiers()` effectue un `release()` pour chaque skieur déchargé, permettant ainsi au thread de se résumer

- Ceci pour s'assurer que les skieurs ne déscendent pas de la cabine en cours de montée, ou ne restent pas bloqués dans la cabine

  

Les nombres de skieurs attendant une cabine ou attendant dans la cabine sont incrémentés par les méthodes `waitForCableCar()` et `waitInsideCableCar()` respectivement.

Afin de minimiser l'attente des skieurs à la station, la cabine est toujours chargée à son maximum, ou au nombre de skieurs attendant si ce nombre est plus bas que la capacité de la cabine.

Les nombres d'attente hors et à l'intérieur de la cabine sont modifiés convenablement dans les méthodes `goIn()` et `goOut()`.

### Gestion de la synchronisation

La synchronisation skieurs-cabine étant décrite au dessus, il nous reste la synchronisation cabine-skieurs.

Afin de nous assurer que la cabine ne commence pas une montée avant que le maximum de skieurs en attente hors de la cabine soient entrés ou avant que tous les skieurs dans la cabine soient sortis, nous utilisont un sémaphore initialisé à 0 comme pour la synchronisation skieurs-cabine.

Ce sémaphore est décrémenté (bloqué) lorsque chaque skieur est chargé ou déchargé de la cabine, et incrémenté lorsqu'il `goIn()` ou `goOut()`.

### Protection des variables

Lors de la décrémentation ou de l'incrémentation des variables nbSkierInside et nbSkierWaiting, nous protégeons leurs accès par deux sémaphores en mode mutex. Ainsi, si plusieurs threads essayent de modifier en même temps ces variables, nous nous assurons qu'elles restent dans un état cohérant par rapport au programme. 

## Tests effectués [TODO]

Cabine fait aller-retours vide?

Fonctionnement général

- pas de dépassement de capacité de la cabine
- les skieurs ne sautent pas de la cabine en plein trajet
- Pas de redescente à vide
- Un skieur pas en avance ne monte pas dans la cabine

