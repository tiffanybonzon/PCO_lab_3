//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Jérôme Arn, Tiffany Bonzon

#include "cablecarbehavior.h"

void CableCarBehavior::run()
{
    while(cableCar->isInService()){
        cableCar->loadSkiers();
        cableCar->goUp();
        cableCar->unloadSkiers();
        cableCar->goDown();
    }
}
