#include "skip_list/skip_list.h"

int main()
{
    /*SkipList<int, int> SList;
    SList.insertNode(1,2);
    SList.insertNode(2,2);
    SList.insertNode(3,2);
    SList.insertNode(5,9);
    SList.insertNode(100,34);
    for(int i = SList.getLevel(); i >= 0; i--)
    {
        SList.displayByLevel(i);
    }
    return 0;
     */
    SkipList<int, int> S;
    for(int i = 0; i < 10; i++)
    {
        cout << S.randomLevel() << endl;
    }

}

