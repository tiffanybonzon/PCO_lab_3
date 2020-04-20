# Laboratoire 4 - station de ski

Auteurs: Jérôme Arn, Tiffany Bonzon

## Description des fonctionnalités du logiciel

Le but de ce programme est de modéliser une station de ski avec des skieurs et un télécabine. Ces derniers sont représentés par des threads. 

Le but est de pouvoir chargé des skieurs dans le télé cabine en respectant sa capacité, et ceci quelque soit le nombre de skieurs qui attendent. Le but est de synchronisé le chargement et déchargement des skieur afin qu'ils ne sortent pas en cours de route. 

## Choix d'implémentation

### Méthode Run télé cabine 

Pendant que le télé cabine est en service, on effectue des chargements, une ascension, un déchargement    et une descente. Quand la station est fermée, le thread quitte la boucle et se termine. 

### Méthode Run Skieur

Pendant que le télé cabine est en service, on attend le télécabine, on rentre dedans, on attend que l'ascension soit effectuée et on sort. Quand la station est fermée soit on finit sa descente pour finir le thread ou alors si on était en attente, on est libéré et on termine le thread. 

### Gestion de l'attente

L'attente se situe au niveau de waitForCableCar() et waitInsideCableCar(). Une sémaphore initialisé à 0, s'incrémente au fur et à mesure que les threads arrivent, quand cela est possible on libère le nombre threads correspondant à la capacité. Pour savoir combien de personne attende, on incrémente nbSkierWaiting à chaque fois qu'un thread arrive. 

La même chose se passe pour l'attente à l'intérieur de la cabine. nbSkierInside contient le nombre effectif de thread dans la cabine.  

### Gestion de la synchronisation

Pour être sur que la cabine ne descend pas ou ne  monte pas avant que tous les threads soient sortit ou entré, nous avons une sémaphore pour attendre la sortie ou l'entrée de chaque thread. 

### protection des variables

Lors de la décrémentation ou de l'incrémentation des variables nbSkierInside et nbSkierWaiting, nous protégeons leurs accès par deux sémaphores en mode mutex. Même si le risque que deux threads modifie en même temps l'accès, nous nous assurons que cela est protéger. 

## Tests effectués

