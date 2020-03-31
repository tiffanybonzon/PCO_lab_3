#include <pcosynchro/pcosemaphore.h>
#ifndef PCOBARRIER_H
#define PCOBARRIER_H

class PcoBarrier
{
protected:

    PcoSemaphore *sem;
    int ToWait;
    int ref;
public:
    PcoBarrier(unsigned int nbToWait)
    {
        sem = new PcoSemaphore(0);
        ToWait = 0;
        ref = nbToWait;
    }

    ~PcoBarrier()
    {
    }

    void wait()
    {
        if(++ToWait < ref){
            sem->acquire();
        }else{
            while(ToWait != 0){
                sem->release();
                ToWait--;
            }
        }
    }
};

#endif // PCOBARRIER_H
