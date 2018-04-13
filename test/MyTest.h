//
// Created by NorSnow_ZJ on 2018/4/11.
//

#ifndef SOME_DATA_STRUCTRUE_TEST_H
#define SOME_DATA_STRUCTRUE_TEST_H

#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include "SpinLockTest.h"

class MyTest
{
public:
    using TestCaseMap = std::unordered_map<std::string, std::shared_ptr<TestCase>>;

    void init()
    {
        //自旋锁测试用例
        registerTestCase("SpinLock", std::make_shared<SpinLockTest>());
    }

    void registerTestCase(const std::string & name, std::shared_ptr<TestCase> && testCase)
    {
        testMap[name] = testCase;
    }

    bool runTestCase(const std::string & name)
    {
        if (testMap.find(name) != testMap.end())
            return testMap[name]->test();
    }

    void runAllTestCases()
    {
        for (auto p : testMap)
        {
            std::cout << "Start test: " << p.first << std::endl;
            std::string msg = p.second->test() ? (p.first + " test passes!") : (p.first + " test failed!");
            std::cout << msg << std::endl;
        }
    }

private:

    TestCaseMap testMap;
};


#endif //SOME_DATA_STRUCTRUE_TEST_H
