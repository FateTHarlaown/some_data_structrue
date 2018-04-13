#include <string>
#include <vector>
#include <cassert>
#include "MyTest.h"

int main()
{
    MyTest test;
    test.init();
    test.runAllTestCases();
    //assert(s_test.MyTest());
    return 0;
    /*
    SkipList<int, int> SList;
    SList.insertNode(1,2);
    SList.insertNode(2,2);
    SList.insertNode(3,2);
    SList.insertNode(5,9);
    SList.insertNode(100,34);
    for(int i = SList.getLevel(); i >= 0; i--)
    {
        SList.displayByLevel(i);
    }
    int ans;
    if (SList.searchNode(5, ans))
    {
        cout << "Find val for key 5:  " << ans << endl;
    }
    else
        cout << "fuck!!" << endl;
    if (SList.deleteNode(3))
    {
        cout << "delete key 3" << endl;
        for(int i = SList.getLevel(); i >= 0; i--)
        {
            SList.displayByLevel(i);
        }
    }
    else
        cout << "Oh no!" << endl;
    if (SList.updateNode(100, -20))
    {
        cout << "delete key 3" << endl;
        for(int i = SList.getLevel(); i >= 0; i--)
        {
            SList.displayByLevel(i);
        }
    }
    else
        cout << "Oh no!" << endl;

    return 0;
     */
}

