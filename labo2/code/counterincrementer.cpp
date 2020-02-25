#include "counterincrementer.h"

static volatile long unsigned int counter;

void runTask(CriticalSection *criticalSection, unsigned long nbIterations, int id)
{
    long unsigned int i = 0;

    while (i < nbIterations)
    {
        counter++;
        i++;
    }
}

void initCounter()
{
    counter = 0;
}

long unsigned int getCounter()
{
    return counter;
}
