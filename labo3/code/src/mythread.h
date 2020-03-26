#ifndef MYTHREAD_H
#define MYTHREAD_H
#include "threadmanager.h"

/**
 * @brief runTask Tâche effectuée par le thread pour cracker le mdp
 * @param tm Référence sur le threadManager ayant lancé le thread
 * @param charset Le charset utilisé pour le cassage su mot de passe courrant
 * @param hash Le hash du mot de passe recherché
 * @param salt Le sel utilisé pour hasher le mot de passe recherché
 * @param nbChars Le nombre de caractères du mot de passe recherché
 * @param min L'index du premier mot de passe à rechercher par le thread
 * @param max L'index du dernier mot de passe à rechercher par le thread
 */
void runTask(ThreadManager *tm, QString charset, QString hash, QString salt, unsigned int nbChars, long long unsigned int min, long long unsigned int max);

/**
 * @brief initThread initialise le compteur général à 0 et met wasFound à false
 */
void initThread();

/**
 * @brief getCounter retourne l'état du compteur
 * @return la valeur du compteur
 */
long long unsigned int getCounter();
#endif // MYTHREAD_H
