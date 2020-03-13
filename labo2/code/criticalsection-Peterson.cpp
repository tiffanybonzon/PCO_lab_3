#include "criticalsection.h"
#include<atomic>
#include<thread>
#include<array>
#include <algorithm>
using namespace std;

void WonderfulCriticalSection::initialize(unsigned int nbThreads){
    this->level.resize(nbThreads);
    this->last_to_enter.resize(nbThreads -1 );
    this->level.assign(nbThreads,-1);
    this->last_to_enter.assign(nbThreads,0);
}

void WonderfulCriticalSection::lock(unsigned int index){
    int k = 0;
    std::atomic_thread_fence(std::memory_order_acq_rel);
    for(size_t l = 0; l < level.size();l++){
        level[index] = l;
        std::atomic_thread_fence(std::memory_order_acq_rel);
        last_to_enter[l] = index;
        std::atomic_thread_fence(std::memory_order_acq_rel);
        while(last_to_enter[l] == (int)index && level[k] >= (int)l){
            std::this_thread::yield();
            k == (int)index? k+=2: k++;
            std::atomic_thread_fence(std::memory_order_acq_rel);
        }
    }
}

void WonderfulCriticalSection::unlock(unsigned int index){
    level[index] = -1;
    std::atomic_thread_fence(std::memory_order_acq_rel);
}
