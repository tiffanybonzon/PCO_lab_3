#include "mythread.h"
#include<thread>
static volatile long unsigned int nbComputed;

void runTask( unsigned long nbToCompute, int id){
    long long unsigned int i = 0;

    while (i < nbToCompute){
        nbComputed++;
        i++;
    }
}

void initCounter(){
    nbComputed = 0;
}

long long unsigned int getCounter(){
    return nbComputed;
}
