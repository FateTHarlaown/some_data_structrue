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
    SpinLockTest() : count_(0)
    {}

    virtual bool test();

private:

    void incCounter()
    {
        for (int i = 0; i < kIncNum; ++i)
        {
            spinLock_.lock();
            count_++;
            spinLock_.unlock();
        }
    }

    const int kIncNum = 1000000;
    const int kWorkerNum = 10;

    volatile int count_;
    SpinLock spinLock_;
    std::vector<std::thread> workers_;
};


#endif //SOME_DATA_STRUCTRUE_SPIN_LOCK_TEST_H
