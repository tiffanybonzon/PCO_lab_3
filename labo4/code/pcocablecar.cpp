//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Jérôme Arn, Tiffany Bonzon

#include "pcocablecar.h"
#include <pcosynchro/pcothread.h>

#include <QDebug>
#include <QRandomGenerator>
constexpr unsigned int MIN_SECONDS_DELAY = 1;
constexpr unsigned int MAX_SECONDS_DELAY = 5;
constexpr unsigned int SECOND_IN_MICROSECONDS = 1000000;
// barrières d'attente dehors et dedans du télécabine
static PcoSemaphore *waitForCar;
static PcoSemaphore *waitInsideCar;
// mode mutex pour protection de zone critique
static PcoSemaphore *protectNbSkierWaiting = new PcoSemaphore(1);
static PcoSemaphore *protectNbSkierInside = new PcoSemaphore(1);
// semaphore d'attente et de synchronisation pour que tous les skieurs soient sortis
static PcoSemaphore *synchro = new PcoSemaphore(0);

/**
 * @brief PcoCableCar::PcoCableCar
 * @param capacity capacité du télécabine
 */
PcoCableCar::PcoCableCar(const unsigned int capacity) : capacity(capacity)
{
    inService = true;
    waitForCar = new PcoSemaphore(0);
    waitInsideCar = new PcoSemaphore(0);
    nbSkiersWaiting=0;
    nbSkiersInside=0;
}

/**
 * @brief PcoCableCar::~PcoCableCar
 */
PcoCableCar::~PcoCableCar()
{

}

/**
 * @brief PcoCableCar::waitForCableCar attente du télécabine
 * @param id du skieur
 */
void PcoCableCar::waitForCableCar(int id)
{
    qDebug() << id << " a froid car il attend";
    protectNbSkierWaiting->acquire();
    nbSkiersWaiting++;
    protectNbSkierWaiting->release();
    waitForCar->acquire();
}

/**
 * @brief PcoCableCar::waitInsideCableCar attente dans le télécabine
 * @param id du skieur
 */
void PcoCableCar::waitInsideCableCar(int id)
{
    qDebug() << id << " attend dans le télé cabine";
    waitInsideCar->acquire();
}

/**
 * @brief PcoCableCar::goIn transition pour le chargement
 * @param id du skieur
 */
void PcoCableCar::goIn(int id)
{
    qDebug() << id << " rentre dans le télé cabine";
    protectNbSkierWaiting->acquire();
    nbSkiersWaiting--;
    protectNbSkierWaiting->release();
    protectNbSkierInside->acquire();
    nbSkiersInside++;
    protectNbSkierInside->release();
    synchro->release();
}

/**
 * @brief PcoCableCar::goOut transition pour le déchargement
 * @param id du skieur
 */
void PcoCableCar::goOut(int id)
{
    qDebug() << id << " sort du télé cabine et va skier";
    protectNbSkierInside->acquire();
    nbSkiersInside--;
    protectNbSkierInside->release();
    synchro->release();
}

/**
 * @brief PcoCableCar::isInService
 * @return si le télé cabine est en service
 */
bool PcoCableCar::isInService()
{
    return inService;
}

/**
 * @brief PcoCableCar::endService libére les skieurs en attente et ferme la station
 */
void PcoCableCar::endService()
{
    // on ferme le télé cabine, on libère les skieurs qui attendent
    inService = false;
    for(unsigned i = 0;i < nbSkiersWaiting;i++){
        waitForCar->release();
    }
}

/**
 * @brief PcoCableCar::goUp le télé cabine monte
 */
void PcoCableCar::goUp()
{
    qDebug() << "Le télécabine monte";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
}

/**
 * @brief PcoCableCar::goDown le télé cabine descend
 */
void PcoCableCar::goDown()
{
    qDebug() << "Le télécabine descend";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
}

/**
 * @brief PcoCableCar::loadSkiers chargement des skieurs et attente que tout le monde soit monté
 */
void PcoCableCar::loadSkiers()
{
    // chargement des skieurs et attente qu'ils soient tous rentrer
    unsigned nbToEnter = nbSkiersWaiting > capacity? capacity: nbSkiersWaiting;
    for(unsigned i = 0;i < nbToEnter;i++){
        waitForCar->release();
        synchro->acquire();
    }
}

/**
 * @brief PcoCableCar::unloadSkiers déchargement des skieurs et attente que tout le monde soit descendu
 */
void PcoCableCar::unloadSkiers()
{
    // sortie des skieurs et attente qu'ils soient tous sorti
    unsigned i = nbSkiersInside;
    for(;i != 0;i--){
        waitInsideCar->release();
        synchro->acquire();
    }
}
