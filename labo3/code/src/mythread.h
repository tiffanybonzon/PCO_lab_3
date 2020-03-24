#ifndef MYTHREAD_H
#define MYTHREAD_H
#include "threadmanager.h"

void runTask(ThreadManager *tm, long long unsigned int min, long long unsigned int max);

void initCounter();

long long unsigned int getCounter();
#endif // MYTHREAD_H
