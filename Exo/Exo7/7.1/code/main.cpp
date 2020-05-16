
#include <iostream>
#include <algorithm>

#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcosemaphore.h>


class Checker
{
public:

    std::vector<int> executions;

    void compute(int id)
    {
        static std::mutex mutex;
        mutex.lock();
        executions.push_back(id);
        mutex.unlock();
        logger() << "Task " << id << std::endl;
    }

    bool executedBefore(int id0, int id1)
    {
        auto it0 = find(executions.begin(), executions.end(), id0);
        auto it1 = find(executions.begin(), executions.end(), id1);
        return it0 < it1;
    }

    void check()
    {
        EXPECT_TRUE(executedBefore(1, 2));
        EXPECT_TRUE(executedBefore(1, 3));
        EXPECT_TRUE(executedBefore(2, 4));
        EXPECT_TRUE(executedBefore(3, 5));
        EXPECT_TRUE(executedBefore(4, 6));
        EXPECT_TRUE(executedBefore(5, 6));
    }
};

static Checker checker;

void t1()
{
    checker.compute(1);

}

void t2()
{
    checker.compute(2);
}

void t3()
{
    checker.compute(3);
}


void t4()
{
    checker.compute(4);
}

void t5()
{
    checker.compute(5);
}

void t6()
{
    checker.compute(6);
}


TEST(Sequentialite, Standard)
{
    PcoManager::getInstance()->setMaxSleepDuration(1000, PcoManager::EventType::ThreadCreation);

    // Il s'agit ici d'un exemple qui n'est pas forcément la solution
    std::vector<std::unique_ptr<PcoThread>> threads(6);
    threads[0] = std::make_unique<PcoThread>(t1);
    threads[0]->join();
    threads[1] = std::make_unique<PcoThread>(t2);
    threads[2] = std::make_unique<PcoThread>(t3);
    threads[1]->join();
    threads[2]->join();
    threads[3] = std::make_unique<PcoThread>(t4);
    threads[4] = std::make_unique<PcoThread>(t5);
    threads[3]->join();
    threads[4]->join();
    threads[5] = std::make_unique<PcoThread>(t6);
    threads[5]->join();

    checker.check();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
