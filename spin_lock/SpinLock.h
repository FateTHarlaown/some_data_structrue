//
// Created by NorSnow_ZJ on 2018/4/11.
//

#ifndef SOME_DATA_STRUCTRUE_SPIN_LOCK_H
#define SOME_DATA_STRUCTRUE_SPIN_LOCK_H

#include <atomic>

class SpinLock {

public:
    SpinLock() : flag_(false)
    {}

    void lock()
    {
        bool expect = false;
        while (!flag_.compare_exchange_weak(expect, true))
        {
            expect = false;
        }
    }

    void unlock()
    {
        flag_.store(false);
    }

private:
    volatile std::atomic<bool> flag_;
};

#endif //SOME_DATA_STRUCTRUE_SPIN_LOCK_H
