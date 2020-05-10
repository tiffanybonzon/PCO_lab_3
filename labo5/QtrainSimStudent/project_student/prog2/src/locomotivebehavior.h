//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Nom Prénom, Nom Prénom
//
#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"
#include "ctrain_handler.h"
#include <QVector>

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive& loco,
                       std::shared_ptr<SharedSectionInterface> sharedSection ,
                       QVector<int> contactList,
                       unsigned AcceptContat1,unsigned  AcceptContact2,
                       unsigned requestContat1,unsigned  requestContact2) :
        loco(loco),
        sharedSection(sharedSection),
        contactList(contactList),
        ACCEPT_CONTACT_1(AcceptContat1), ACCEPT_CONTACT_2(AcceptContact2),
        REQUEST_CONTACT_1(requestContat1),REQUEST_CONTACT_2(requestContact2){
        priority = loco.priority > 0? SharedSectionInterface::Priority::HighPriority : SharedSectionInterface::Priority::HighPriority;
    }

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    QVector<int> contactList;

private:
    void getAccessSS(unsigned int actualContact, unsigned int pointsTD, unsigned int pointsD);
    // contacts auquel on accepte ou non l'accès
    const unsigned int ACCEPT_CONTACT_1;
    const unsigned int ACCEPT_CONTACT_2;
    // contacts auquel on demande l'accès
    const unsigned int REQUEST_CONTACT_1;
    const unsigned int REQUEST_CONTACT_2;
    // garde une trace de la demande de bloquage de la section paratagée
    bool sharedSectionRequested = false;
    // conversion en niveau de priorité
    SharedSectionInterface::Priority priority;
};

#endif // LOCOMOTIVEBEHAVIOR_H
