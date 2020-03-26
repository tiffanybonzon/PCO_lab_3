#include "mythread.h"
#include <pcosynchro/pcomutex.h>
#include <QString>
#include <QCryptographicHash>
#include <QVector>
static volatile long long unsigned int nbComputed;
static volatile bool wasFound;

void runTask(ThreadManager *tm, QString charset, QString hash, QString salt, unsigned int nbChars, long long unsigned int min, long long unsigned int max){
    // mutex pour section critique
    PcoMutex mut;

    long long unsigned int modulo = min / charset.length();
    while(modulo >= (unsigned int)charset.length()) modulo /= charset.length();

    QVector<unsigned int> currentPasswordArray;
    currentPasswordArray.fill(0,nbChars);
    currentPasswordArray[nbChars - 1] = modulo;

    QString currentPasswordString;
    // Mot de passe à tester courant
    for (unsigned int j=0;j < nbChars;j++)
        currentPasswordString[j]  = charset.at(currentPasswordArray.at(j));

    QString currentHash;//Hash du mot de passe à tester courant
    QCryptographicHash md5(QCryptographicHash::Md5);//Object QCryptographicHash servant à générer des md5

    while(min < max && !wasFound){
        md5.reset();/* On vide les données déjà ajoutées au générateur */
        /* On préfixe le mot de passe avec le sel */
        md5.addData(salt.toLatin1());
        md5.addData(currentPasswordString.toLatin1());
        currentHash = md5.result().toHex(); /* On calcul le hash */

        if (currentHash == hash){
            tm->setPassword(currentPasswordString);
            wasFound = true;
            return;
        }

        unsigned int i = 0;
        while (i < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[i]++;

            if (currentPasswordArray[i] >= (unsigned int)charset.length()) {
                currentPasswordArray[i] = 0;
                i++;
            } else
                break;
        }
        //On traduit les index présents dans currentPasswordArray en caractères
        for (unsigned int j =0;j < nbChars;j++)
            currentPasswordString[j]  = charset.at(currentPasswordArray.at(j));
        mut.lock();
        nbComputed++;
        mut.unlock();
        min++;
    }
}

void initThread(){
    nbComputed = 0;
    wasFound = false;
}

long long unsigned int getCounter(){
    // lecture atomique donc pas besoin de protection
    long long unsigned int tmp = nbComputed;
    return tmp;
}
