//
// Created by NorSnow_ZJ on 2018/4/11.
//

#include "spin_lock_test.h"
#include <iostream>

bool spin_lock_test::test()
{
    std::cout << "spin_lock inc test start" << std::endl;
    count = 0;

    std::cout << "start " << kWorkerNum << " workers" << std::endl;
    std::cout << "count: " << count << std::endl;

    for (int i = 0; i < kWorkerNum; ++i)
    {
        std::thread worker(std::bind(spin_lock_test::incCounter, this));
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
        std::cout << "spin_lock inc test passed" << std::endl;
        return true;
    }
    else
    {
        std::cout << "spin_lock inc test failed" << std::endl;
        return false;
    }
}
