
#include <pcosynchro/pcosemaphore.h>


class BridgeManagerFloat
{
private:
    PcoSemaphore *mutex;
    PcoSemaphore *wait;
    float maxWeight;
    float actualWeight;
public:
    BridgeManagerFloat(float maxWeight)
    {
        this->maxWeight = maxWeight;
        this->actualWeight = 0.;
        this->mutex = new PcoSemaphore(1);
        this->wait = new PcoSemaphore(0);
    }

    ~BridgeManagerFloat()
    {

    }

    void access(float weight)
    {
        // tant que le véhicule n'est pas passer, essayer de passer et que son poids est inférieur au poids maximum
        while(weight <= this->maxWeight){
            if((this->actualWeight + weight) > this->maxWeight){
                wait->acquire();
                continue;
            }else{
                mutex->acquire();
                this->actualWeight += weight;
                mutex->release();
                break;
            }
        }
    }

    void leave(float weight)
    {
        mutex->acquire();
        this->actualWeight -= weight;
        mutex->release();
        // indique qu'un autre véhicule peut tenter le passage
        wait->release();
    }
};
