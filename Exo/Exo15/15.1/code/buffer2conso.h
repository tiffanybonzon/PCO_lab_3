#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include <pcosynchro/pcosemaphore.h>
#include <QDebug>
#include "abstractbuffer.h"

template<typename T> class Buffer2ConsoSemaphore : public AbstractBuffer<T> {
protected:
    std::vector<T> tab;
    PcoSemaphore waitNotEmpty;
    PcoSemaphore waitNotFull;
    PcoSemaphore mutex;
public:
    Buffer2ConsoSemaphore(): mutex(1), waitNotEmpty(0), waitNotFull(2){
        tab.resize(1);
    }

    virtual ~Buffer2ConsoSemaphore() {}

    virtual void put(T item) {
        waitNotFull.acquire();
        waitNotFull.acquire();
        mutex.acquire();
        tab.at(0) = item;
        waitNotEmpty.release();
        waitNotEmpty.release();
        mutex.release();
    }
    virtual T get(void) {
        T item;
        waitNotEmpty.acquire();
        mutex.acquire();
        item = tab.at(0);
        waitNotFull.release();
        mutex.release();
        return item;
    }
};


#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

template<typename T> class Buffer2ConsoMesa : public AbstractBuffer<T> {
protected:
    PcoMutex mutex;

public:
    Buffer2ConsoMesa() {}

    virtual ~Buffer2ConsoMesa() {}

    virtual void put(T item) {}
    virtual T get(void) {}
};


#include <pcosynchro/pcohoaremonitor.h>

template<typename T> class Buffer2ConsoHoare : public AbstractBuffer<T>, public PcoHoareMonitor {
protected:

public:
    Buffer2ConsoHoare() {}

    virtual ~Buffer2ConsoHoare() {}

    virtual void put(T item) {}
    virtual T get(void) {}
};


#endif // BUFFER2CONSO_H
