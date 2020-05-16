#include <pcosynchro/pcosemaphore.h>
#include <iostream>


class MutexFromSem
{
protected:

    bool isLock;
    PcoSemaphore *sem ;
public:
    MutexFromSem()
    {
        sem = new PcoSemaphore(1);
        isLock = false;
    }

    ~MutexFromSem()
    {
    }

    void lock()
    {
        sem->acquire();
        isLock = true;
    }

    void unlock()
    {
        sem->release();
        isLock = false;
    }

    bool trylock()
    {
        return isLock;
    };  //! Returns true if the mutex can be acquired, false if it is already locked
};
