#include <QCryptographicHash>
#include <QVector>
#include <pcosynchro/pcothread.h>
#include "threadmanager.h"
#include "mythread.h"
long long unsigned int nbComputed;
unsigned int nbValidChars;//Nombre de caractères différents pouvant composer le mot de passe
long long unsigned int nbToCompute;
QVector<unsigned int> currentPasswordArray;
QString password = "";

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

void testHash(QString currentPasswordString, QString hash, QString salt){
    QString currentHash;//Hash du mot de passe à tester courant
    QCryptographicHash md5(QCryptographicHash::Md5);//Object QCryptographicHash servant à générer des md5
    md5.reset();/* On vide les données déjà ajoutées au générateur */
    /* On préfixe le mot de passe avec le sel */
    md5.addData(salt.toLatin1());
    md5.addData(currentPasswordString.toLatin1());
    currentHash = md5.result().toHex(); /* On calcul le hash */

    /*
     * Si on a trouvé, on retourne le mot de passe courant (sans le sel)
     */
    if (currentHash == hash){
        password= currentPasswordString;
    }
}

void genPass(unsigned int nbChars, QString salt,QString hash, QString charset, unsigned int min, unsigned int max){
    PcoMutex mut;
    /*
     * On récupère le mot de pass à tester suivant.
     *
     * L'opération se résume à incrémenter currentPasswordArray comme si
     * chaque élément de ce vecteur représentait un digit d'un nombre en
     * base nbValidChars.
     *
     * Le digit de poids faible étant en position 0
     */
    // init de division du mot de passe
    QString currentPasswordString;// Mot de passe à tester courant
    currentPasswordString.fill(charset.at(0),nbChars);
    currentPasswordString[0] = charset.at(min);
    // variable de boucle
    unsigned i = 0;
    // init de la version numerique du mot de passe
    currentPasswordArray.fill(0,nbChars);
    currentPasswordArray[0] = min;

    while (currentPasswordArray[0] <= max){
        while (i < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[i]++;

            if (currentPasswordArray[i] >= nbValidChars) {
                currentPasswordArray[i] = 0;
                i++;
            } else
                break;
        }

        /*
         * On traduit les index présents dans currentPasswordArray en
         * caractères
         */
        for (i=0;i<nbChars;i++)
            currentPasswordString[i]  = charset.at(currentPasswordArray.at(i));
        mut.lock();
        nbComputed++;
        mut.unlock();
        testHash(currentPasswordString,hash,salt);
    }
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
    nbToCompute        = intPow(charset.length(),nbChars);
    nbComputed         = 0;
    nbValidChars       = charset.length();//Nombre de caractères différents pouvant composer le mot de passe

    /* Crée les threads, on ajoutant leur pointeur à la liste.
       Les threads sont immédiatement lancés par le constructeur. */
    for (long unsigned int i=0; i<nbThreads; i++){
        PcoThread *currentThread = new PcoThread(genPass, nbChars, salt, hash,charset, 0, nbChars);
        threadList.push_back(std::unique_ptr<PcoThread>(currentThread));
    }
    while (nbComputed < nbToCompute) {

        /*
         * Tous les 1000 hash calculés, on notifie qui veut bien entendre
         * de l'état de notre avancement (pour la barre de progression)
         */
        if ((nbComputed % 1000) == 0) {
            incrementPercentComputed((double)1000/nbToCompute);
        }




        genPass(nbChars, salt, hash,charset, 0, nbChars);
    }

    /* Attends la fin de chaque thread et libère la mémoire associée.
     * Durant l'attente, l'application est bloquée.
     */
    for (long unsigned int i=0; i<nbThreads; i++){
        threadList[i]->requestStop();
        threadList[i]->join();
    }
    /* Vide la liste de pointeurs de threads */
    threadList.clear();
    /*
     * Si on arrive ici, cela signifie que tous les mot de passe possibles ont
     * été testés, et qu'aucun n'est la préimage de ce hash.
     */
    return QString("");
}
