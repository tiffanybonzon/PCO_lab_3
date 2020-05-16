#include <iostream>

#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include <pcosynchro/pcohoaremonitor.h>

class SemaphoreFromMesaMonitor
{

public:
    SemaphoreFromMesaMonitor(unsigned int initValue)
    {

    }

    ~SemaphoreFromMesaMonitor()
    {

    }

    void wait()
    {
    }

    void post()
    {
    }

    bool trywait()
    {
        return false;
    }  //! Returns true if the semaphore can be acquired, false if it is already blocked
};



class SemaphoreFromHoareMonitor : public PcoHoareMonitor
{
public:
    SemaphoreFromHoareMonitor(unsigned int initValue)
    {

    }

    ~SemaphoreFromHoareMonitor()
    {

    }

    // Attention ici, PcoHoareMonitor a une méthode wait(condition).
    // Pour l'appeler, utilisez PcoHoareMonitor::wait(condition);

    void wait()
    {
    }

    void post()
    {
    }

    bool trywait()
    {
        return false;
    }  //! Returns true if the semaphore can be acquired, false if it is already blocked
};
