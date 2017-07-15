//
// Created by 兔晓侠 on 2017/7/9.
//

#ifndef SOME_DATA_STRUCTRUE_SKIP_LIST_H_H
#define SOME_DATA_STRUCTRUE_SKIP_LIST_H_H

#include <vector>


using namespace std;

template <class KeyType, class DataType>
class ListNode
{
public:
    ListNode(){}
    ListNode(KeyType DefaultKey, DataType DefaultValue):key(DefaultKey),
                                                        value(DefaultValue)
    {

    }
    DataType value;
    KeyType key;
    vector<ListNode*> forwards;//pointers to node from different level
};

template <class KeyType, class DataType>
class SkipList
{
public:
    SkipList(int maxLevel, float skipListP = 0.25);
    bool insertNode(KeyType key, DataType value);
    bool searchNode(KeyType key, DataType & value);
    bool updateNode(KeyType key, DataType newValue);
    bool deleteNode(KeyType key);
    int randomLevel();
private:
    const int MAX_LEVEL;
    const float SKIP_LIST_P;
    int level_num;//从0开始编号
    ListNode<DataType, KeyType> head;
};

#endif //SOME_DATA_STRUCTRUE_SKIP_LIST_H_H

