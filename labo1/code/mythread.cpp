#include "mythread.h"

static volatile long unsigned int counter;
static volatile bool IsRunning = false;

void runTask(unsigned long nbIterations)
{
    long unsigned int i = 0;
    // active  waiting until the other thread are running
    while (IsRunning) ;
    IsRunning = true;
    while (i < nbIterations)
    {
        counter++;
        i++;
    }
    IsRunning = false;
}

void initCounter()
{
    counter = 0;
}

long unsigned int getCounter()
{
    return counter;
}
