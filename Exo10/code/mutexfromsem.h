#include <pcosynchro/pcosemaphore.h>
#include <iostream>


class MutexFromSem
{
protected:

public:
    MutexFromSem()
    {

    }

    ~MutexFromSem()
    {

    }

    void lock()
    {
    }

    void unlock()
    {
    }

    bool trylock()
    {
    };  //! Returns true if the mutex can be acquired, false if it is already locked
};
