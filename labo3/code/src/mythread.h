#ifndef MYTHREAD_H
#define MYTHREAD_H
#include "threadmanager.h"

void runTask(ThreadManager *tm, unsigned int min, unsigned int max);

void initCounter();

long long unsigned int getCounter();
#endif // MYTHREAD_H
