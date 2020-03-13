#include "criticalsection.h"
#include<atomic>
#include<thread>
#include<array>
#include <algorithm>
using namespace std;

void WonderfulCriticalSection::initialize(unsigned int nbThreads)
{
    vectorInt.assign(nbThreads,0);
    vectorBool.assign(nbThreads, false);
}

void WonderfulCriticalSection::lock(unsigned int index)
{
    vectorBool[index] = true;
    std::atomic_thread_fence(std::memory_order_acq_rel);
    vectorInt[index] = 1 + *max_element(vectorInt.begin(), vectorInt.end());
    std::atomic_thread_fence(std::memory_order_acq_rel);
    vectorBool[index] = false;
    std::atomic_thread_fence(std::memory_order_acq_rel);

    for (size_t j = 0; j < vectorInt.size(); j++) {
        if(j != index){
            while (vectorBool[j]){
                std::this_thread::yield();
            }
            std::atomic_thread_fence(std::memory_order_acq_rel);
            while ((vectorInt[j] != 0)
                   && ( vectorInt.at(index) > vectorInt.at(j)||
                        (vectorInt.at(index) == vectorInt.at(j) &&
                         index > j))){
                std::this_thread::yield();
            }
        }
    }

}

void WonderfulCriticalSection::unlock(unsigned int index)
{
    this->vectorInt[index] = 0;
    std::atomic_thread_fence(std::memory_order_acq_rel);
}
