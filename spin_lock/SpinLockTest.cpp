//
// Created by NorSnow_ZJ on 2018/4/11.
//

#include "SpinLockTest.h"
#include <iostream>

bool SpinLockTest::test()
{
    std::cout << "SpinLock inc MyTest start" << std::endl;
    count = 0;

    std::cout << "start " << kWorkerNum << " workers" << "every worker inc " << kIncNum << std::endl;
    std::cout << "count: " << count << std::endl;

    for (int i = 0; i < kWorkerNum; ++i)
    {
        std::thread worker(std::bind(SpinLockTest::incCounter, this));
        workers.push_back(std::move(worker));
    }

    for (auto it = workers.begin(); it != workers.end(); it++)
    {
        it->join();
    }

    std::cout << "workers end" << std::endl;
    std::cout << "count: " << count << std::endl;
    if (count == kIncNum * kWorkerNum)
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
