# EXO 3

Les résultats possible sont 0,1,2,3,4 car chacune des modifications de la variable **nombre** peut intervenir à tout moment. 

# EXO 4

Si le Mutex est static, cela veut dire qu'il sera commun à tous les threads. Donc si un des threads lock le mutex, les autres threads ne pourront pas refaire un lock la-dessus. Dans le deuxième cas. le mutex est partagé entre tous comme ça on a une vrai section crirtique. 

