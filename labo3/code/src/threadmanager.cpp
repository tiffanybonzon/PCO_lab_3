#include <QCryptographicHash>
#include <QVector>
#include "threadmanager.h"
#include "mythread.h"
/*
 * std::pow pour les long long unsigned int
 */
long long unsigned int intPow (
        long long unsigned int number,
        long long unsigned int index)
{
    long long unsigned int i;

    if (index == 0)
        return 1;

    long long unsigned int num = number;

    for (i = 1; i < index; i++)
        number *= num;

    return number;
}

ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent)
{}

void ThreadManager::incrementPercentComputed(double percentComputed)
{
    emit sig_incrementPercentComputed(percentComputed);
}

QString ThreadManager::getHash(){
    return this->hash;
}

void ThreadManager::setPassword(QString password){
    this->password = password;
}

QString ThreadManager::getSalt(){
    return this->salt;
}

QString ThreadManager::getCharSet(){
    return this->charset;
}

QString ThreadManager::getPassword(){
    return this->password;
}

unsigned int ThreadManager::getnbChars(){
    return this->nbChars;
}

unsigned int ThreadManager::getNbValidChars(){
    return this->nbValidChars;
}

unsigned int ThreadManager::getnbToCompute(){
    return this->nbToCompute;
}

/*
 * Les paramètres sont les suivants:
 *
 * - charset:   QString contenant tous les caractères possibles du mot de passe
 * - salt:      le sel à concaténer au début du mot de passe avant de le hasher
 * - hash:      le hash dont on doit retrouver la préimage
 * - nbChars:   le nombre de caractères du mot de passe à bruteforcer
 * - nbThreads: le nombre de threads à lancer
 *
 * Cette fonction doit retourner le mot de passe correspondant au hash, ou une
 * chaine vide si non trouvé.
 */
QString ThreadManager::startHacking(QString charset, QString salt,QString hash,unsigned int nbChars,unsigned int nbThreads){
    /*
     * Calcul du nombre de hash à générer
     */
    this->nbToCompute           = intPow(charset.length(),nbChars);
    this->nbValidChars          = charset.length();//Nombre de caractères différents pouvant composer le mot de passe
    this->hash                  = hash;
    this->charset               = charset;
    this->salt                  = salt;
    this->nbChars               = nbChars;
    this->password              = "";

    initCounter();

    // nombre de calculs par thread
    long long unsigned int  step = this->nbToCompute/nbThreads;

    /* Crée les threads, on ajoutant leur pointeur à la liste.
       Les threads sont immédiatement lancés par le constructeur. */
    long long unsigned int min = 0;
    long long unsigned int max = 0;

    /*
     * Comme nous désirons diviser le nombre de calcul en modifiant le caractère de poids fort de commencement du thread
     * (Exemple aaaH pour 2 threads et 4 caractères) il se peut que la divison ne soit possible qu'en modifiant tous les caractères de début
     * Il faudrait effectuer une partie calcul suplémentaire dans le thread, ce qui n'est pas souhaité.
     * ICI nous replacons juste les intervals pour que tous le charset soit couvert
     */
    long double modulo = step / this->getNbValidChars();
    while(modulo >= this->getNbValidChars()) modulo /= this->getNbValidChars();
    if((modulo - (long long unsigned int)modulo) > 0){
        step = (long long unsigned int)(modulo + 1) * intPow((long long unsigned int)this->nbValidChars, nbChars - 1);
    }

    for (long unsigned int i=0; i<nbThreads; i++){
        // si c'est le dernier thread on fixe le plafond au maximum
        if (i == (nbThreads - 1)){
            max = this->nbToCompute;
        }else{
            max += step;
        }
        PcoThread *currentThread = new PcoThread(runTask, this, min, max);
        threadList.push_back(std::unique_ptr<PcoThread>(currentThread));
        min = max;
    }

    while (getCounter() < nbToCompute && this->password == "") {
        /*
         * Tous les 1000 hash calculés, on notifie qui veut bien entendre
         * de l'état de notre avancement (pour la barre de progression)
         */
        if ((getCounter() % 1000) == 0) {
            incrementPercentComputed((double)1000/nbToCompute);
        }

    }

    /* Attends la fin de chaque thread et libère la mémoire associée.
     * Durant l'attente, l'application est bloquée.
     */
    for (long unsigned int i=0; i<nbThreads; i++){
        threadList[i]->join();
    }

    if(this->password != ""){
        return password;
    }
    /*
     * Si on arrive ici, cela signifie que tous les mot de passe possibles ont
     * été testés, et qu'aucun n'est la préimage de ce hash.
     */
    return QString("");
}
