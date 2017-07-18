#include "skip_list/skip_list.h"

int main()
{
    srand((unsigned int)time(NULL));
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
    if (SList.searchNode(100, ans))
    {
        cout << "Find val for key 5:  " << ans << endl;
    }
    else
        cout << "fuck!!" << endl;
    return 0;

}

