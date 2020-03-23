#include "mythread.h"
#include <pcosynchro/pcomutex.h>
#include <QString>
#include <QCryptographicHash>
#include <QVector>
static volatile long long unsigned int nbComputed;

void runTask(ThreadManager *tm, unsigned int min, unsigned int max){
    // variable de boucle
    unsigned int i = 0;
    // mutex pour section critique
    PcoMutex mut;
    // Mot de passe à tester courant
    QString currentPasswordString;

    // init de division du mot de passe
    currentPasswordString.fill(tm->getCharSet().at(0),tm->getnbChars());
    currentPasswordString[0] = tm->getCharSet().at(0);

    QVector<unsigned int> currentPasswordArray;
    currentPasswordArray.fill(0,tm->getnbChars());
    currentPasswordArray[0] = 0;

    QString currentHash;//Hash du mot de passe à tester courant
    QCryptographicHash md5(QCryptographicHash::Md5);//Object QCryptographicHash servant à générer des md5

    while(min < max){
        md5.reset();/* On vide les données déjà ajoutées au générateur */
        /* On préfixe le mot de passe avec le sel */
        md5.addData(tm->getSalt().toLatin1());
        md5.addData(currentPasswordString.toLatin1());
        currentHash = md5.result().toHex(); /* On calcul le hash */
        i = 0;

        if (currentHash == tm->getHash()){
            tm->setPassword(currentPasswordString);
            return;
        }

        while (i < (unsigned int)currentPasswordArray.size()) {
            currentPasswordArray[i]++;

            if (currentPasswordArray[i] >= tm->getNbValidChars()) {
                currentPasswordArray[i] = 0;
                i++;
            } else
                break;
        }
        /*
         * On traduit les index présents dans currentPasswordArray en caractères
         */
        for (i=0;i<tm->getnbChars();i++)
            currentPasswordString[(int)i]  = tm->getCharSet().at(currentPasswordArray.at(i));

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
    return nbComputed;
}
