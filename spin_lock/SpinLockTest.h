//
// Created by NorSnow_ZJ on 2018/4/11.
//

#ifndef SOME_DATA_STRUCTRUE_SPIN_LOCK_TEST_H
#define SOME_DATA_STRUCTRUE_SPIN_LOCK_TEST_H

#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include "SpinLock.h"
#include "TestCase.h"

class SpinLockTest : public TestCase
{
public:
    SpinLockTest() : count(0)
    {}

    virtual bool test();

private:

    void incCounter()
    {
        for (int i = 0; i < kIncNum; ++i)
        {
            spinLock.lock();
            count++;
            spinLock.unlock();
        }
    }

    const int kIncNum = 1000000;
    const int kWorkerNum = 10;

    volatile int count;
    SpinLock spinLock;
    std::vector<std::thread> workers;
};


#endif //SOME_DATA_STRUCTRUE_SPIN_LOCK_TEST_H
