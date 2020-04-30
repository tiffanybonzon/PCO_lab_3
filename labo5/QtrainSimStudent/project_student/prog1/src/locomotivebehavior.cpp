//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Nom Prénom, Nom Prénom
//
#include "locomotivebehavior.h"
#include "ctrain_handler.h"

#define NB_TOUR_CHANGEMENT_SENS 2

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->request(loco);
    //sharedSection->getAccess(loco);
    //sharedSection->leave(loco);


    // la loco début par attendre le premier prochain contact
    attendre_contact(contactList.at(0));
    while(1) {
        // effectue NB_TOUR_CHANGEMENT_SENS fois un tour de circuit
        for(int i = 0; i < NB_TOUR_CHANGEMENT_SENS; i ++)
            for(int j = 1; j < contactList.size(); j++)
                attendre_contact(contactList.at(j));

        loco.inverserSens();

        // effectue NB_TOUR_CHANGEMENT_SENS fois un tour de circuit
        for(int i = 0; i < NB_TOUR_CHANGEMENT_SENS; i ++)
            for(int j = contactList.size() - 2; j >= 0; j--)
                attendre_contact(contactList.at(j));

        loco.inverserSens();
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}
