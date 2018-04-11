//
// Created by NorSnow_ZJ on 2018/4/11.
//

#ifndef SOME_DATA_STRUCTRUE_SPIN_LOCK_H
#define SOME_DATA_STRUCTRUE_SPIN_LOCK_H

#include <atomic>

class spin_lock {

public:
    spin_lock() : flag(false)
    {}

    void lock()
    {
        bool expect = false;
        while (!flag.compare_exchange_weak(expect, true))
        {
            expect = false;
        }
    }

    void unlock()
    {
        flag.store(false);
    }

private:
    volatile std::atomic<bool> flag;
};

#endif //SOME_DATA_STRUCTRUE_SPIN_LOCK_H
