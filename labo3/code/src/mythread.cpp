#include "mythread.h"
#include <pcosynchro/pcomutex.h>
#include <QString>
#include <QCryptographicHash>
#include <QVector>
static volatile long long unsigned int nbComputed;

void runTask(ThreadManager *tm, long long unsigned int min, long long unsigned int max){
    // mutex pour section critique
    PcoMutex mut;

    long long unsigned int modulo = min / tm->getNbValidChars();
    while(modulo >= tm->getNbValidChars()) modulo /= tm->getNbValidChars();

    QVector<unsigned int> currentPasswordArray;
    currentPasswordArray.fill(0,tm->getnbChars());
    currentPasswordArray[tm->getnbChars() - 1] = modulo;

    QString currentPasswordString;
    // Mot de passe à tester courant
    for (unsigned int j=0;j<tm->getnbChars();j++)
        currentPasswordString[j]  = tm->getCharSet().at(currentPasswordArray.at(j));

    QString currentHash;//Hash du mot de passe à tester courant
    QCryptographicHash md5(QCryptographicHash::Md5);//Object QCryptographicHash servant à générer des md5

    while(min < max){
        md5.reset();/* On vide les données déjà ajoutées au générateur */
        /* On préfixe le mot de passe avec le sel */
        md5.addData(tm->getSalt().toLatin1());
        md5.addData(currentPasswordString.toLatin1());
        currentHash = md5.result().toHex(); /* On calcul le hash */

        if(tm->getPassword() != "")
            return;

        if (currentHash == tm->getHash()){
            tm->setPassword(currentPasswordString);
            return;
        }

        unsigned int i = 0;
        while (i < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[i]++;

            if (currentPasswordArray[i] >= tm->getNbValidChars()) {
                currentPasswordArray[i] = 0;
                i++;
            } else
                break;
        }
        //On traduit les index présents dans currentPasswordArray en caractères
        for (unsigned int j =0;j<tm->getnbChars();j++)
            currentPasswordString[j]  = tm->getCharSet().at(currentPasswordArray.at(j));

        // section critique
        mut.lock();
        nbComputed++;
        mut.unlock();
        min++;
    }
}

void initCounter(){
    nbComputed = 0;
}

long long unsigned int getCounter(){
    // mutex pour section critique
    PcoMutex mut;
    // section critique
    mut.lock();
    long long unsigned int tmp = nbComputed;
    mut.unlock();
    return tmp;
}
