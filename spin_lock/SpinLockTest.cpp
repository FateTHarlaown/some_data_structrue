//
// Created by NorSnow_ZJ on 2018/4/11.
//

#include "SpinLockTest.h"
#include <iostream>

bool SpinLockTest::test()
{
    std::cout << "SpinLock inc MyTest start" << std::endl;
    count_ = 0;

    std::cout << "start " << kWorkerNum << " workers_" << "every worker inc " << kIncNum << std::endl;
    std::cout << "count_: " << count_ << std::endl;

    for (int i = 0; i < kWorkerNum; ++i)
    {
        std::thread worker(std::bind(SpinLockTest::incCounter, this));
        workers_.push_back(std::move(worker));
    }

    for (auto it = workers_.begin(); it != workers_.end(); it++)
    {
        it->join();
    }

    std::cout << "workers_ end" << std::endl;
    std::cout << "count_: " << count_ << std::endl;
    if (count_ == kIncNum * kWorkerNum)
    {
        std::cout << "SpinLock inc MyTest passed" << std::endl;
        return true;
    }
    else
    {
        std::cout << "SpinLock inc MyTest failed" << std::endl;
        return false;
    }
}
