
#include <pcosynchro/pcosemaphore.h>

#define CARWEIGHT 1
#define TRUCKWEIGHT 10


class BridgeManager
{
protected:
    PcoSemaphore *sem;
    PcoSemaphore *mut = new PcoSemaphore(1);
    int max;
    int actual;
public:
    BridgeManager(unsigned int maxWeight)
    {
        sem = new PcoSemaphore(maxWeight);
        actual = 0;
        max = maxWeight;
    }

    ~BridgeManager()
    {

    }

    void carAccess()
    {
        if(actual < max){
            sem->acquire();
            mut->acquire();
            actual++;
            mut->release();
        }
    }

    void truckAccess()
    {
        if((actual+10) <= max){
            for(int i = 0; i < 10;i++){
                sem->acquire();
            }
            mut->acquire();
            actual += 10;
            mut->release();
        }
    }

    void carLeave()
    {
        if(actual > 0){
            sem->release();
            mut->acquire();
            actual--;
            mut->release();
        }
    }

    void truckLeave()
    {
        if(actual > 0){
            for(int i = 0; i < 10;i++){
                sem->release();
            }
            mut->acquire();
            actual -= 10;
            mut->release();
        }
    }
};
