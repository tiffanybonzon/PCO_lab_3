
#include <pcosynchro/pcosemaphore.h>

#include "vehicle.h"

class BridgeManagerFloat
{
private:
    float maxWeight;
    float actualWeight;
    PcoSemaphore *mutex;
    PcoSemaphore *wait;
public:
    BridgeManagerFloat(float maxWeight)
    {
        this->maxWeight = maxWeight;
        this->actualWeight = 0;
        this->mutex = new PcoSemaphore(1);
    }

    ~BridgeManagerFloat()
    {

    }

    void access(Vehicle *vehicle)
    {
        while(true){
            if((actualWeight + vehicle->getWeight()) > maxWeight){
                vehicle->stop();
                wait->acquire();
            }else{
                vehicle->start();
                mutex->acquire();
                actualWeight += vehicle->getWeight();
                mutex->release();
                break;
            }
        }
    }

    void leave(Vehicle *vehicle)
    {
        mutex->acquire();
        actualWeight -= vehicle->getWeight();
        mutex->release();
        // indique qu'un autre véhicule peut tenter le passage
        wait->release();
        vehicle->start();
    }
};
