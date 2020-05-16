#include <iostream>

#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include <pcosynchro/pcohoaremonitor.h>

class SemaphoreFromMesaMonitor
{
private:
    PcoMutex mutex;
    PcoConditionVariable cond;
    unsigned int value;
public:
    SemaphoreFromMesaMonitor(unsigned int initValue): value(initValue)
    {
    }

    ~SemaphoreFromMesaMonitor()
    {
    }

    void wait()
    {
        mutex.lock();
        if (value == 0)
            cond.wait(&mutex);
        value --;
        mutex.unlock();
    }

    void post()
    {
        mutex.lock();
        value++;
        mutex.unlock();
        cond.notifyOne();
    }

    bool trywait()
    {
        return value > 0;
    }  //! Returns true if the semaphore can be acquired, false if it is already blocked
};



class SemaphoreFromHoareMonitor : public PcoHoareMonitor
{
private:
    PcoMutex mutex;
    PcoConditionVariable cond;
    unsigned int value;
public:
    SemaphoreFromHoareMonitor(unsigned int initValue):value(initValue)
    {
    }

    ~SemaphoreFromHoareMonitor()
    {
    }

    void wait()
    {
        mutex.lock();
        while(value == 0)
            cond.wait(&mutex);
        value --;
        mutex.unlock();
    }

    void post()
    {
        mutex.lock();
        value++;
        mutex.unlock();
        cond.notifyOne();
    }

    bool trywait()
    {
        return value > 0;
    }  //! Returns true if the semaphore can be acquired, false if it is already blocked
};

