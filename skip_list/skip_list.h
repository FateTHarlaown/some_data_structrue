//
// Created by 兔晓侠 on 2017/7/9.
//

#ifndef SOME_DATA_STRUCTRUE_SKIP_LIST_H_H
#define SOME_DATA_STRUCTRUE_SKIP_LIST_H_H

#define  DEBUG 1
#include <c++/cstdlib>
#include <algorithm>
#include <iostream>
#include <time.h>
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
    SkipList(int maxLevel = 4, float skipListP = 0.25);
    bool searchNode(const KeyType & key, DataType & value);
    bool insertNode(const KeyType &key, const DataType &value);
    bool updateNode(const KeyType & key, const DataType & newValue);
    bool deleteNode(const KeyType & key);
    int getLevel(){ return level_num;}
    int randomLevel();
#if defined(DEBUG)
    void displayByLevel(int n);
#endif

private:
    const int MAX_LEVEL;
    const float SKIP_LIST_P;
    int level_num;//从0开始编号
    ListNode<DataType, KeyType> head;
};

//implemention

template <class KeyType, class DataType>
SkipList<KeyType, DataType>::SkipList(int maxLevel, float skipListP):MAX_LEVEL(maxLevel),
                                                                     SKIP_LIST_P(skipListP)
{
    level_num = 0;
//无论如何至少有第0层，初始时第0层只有一个头结点
    head.forwards.push_back(NULL);
}


template <class KeyType, class DataType>
bool SkipList<KeyType, DataType>::searchNode(const KeyType & key, DataType & value)
{
    ListNode<KeyType, DataType> * x = &head;
    for(int i = level_num; i >= 0; i--)
    {
        while (x->forwards[i] != NULL && x->forwards[i]->key < key)//这意味着KeyType类型必须有小于运算符“<”，若没有
        {                                                          //或者其表现与所需不符合，需要自己重载该运算符
            x = x->forwards[i];
        }
    }

    x = x->forwards[0];
    if(x != NULL && x->key == key)//存在该节点，返回其值
    {
        value = x->value;
        return true;
    }
    else
    {
        return false;
    }
}

template <class KeyType, class DataType>
bool SkipList<KeyType, DataType>::insertNode(const KeyType &key, const DataType &value)
{
    ListNode<KeyType, DataType> * x = &head;
    vector<ListNode<KeyType, DataType>*> insertPos;
    for(int i = level_num; i >= 0; i--)
    {
        while (x->forwards[i] != NULL && x->forwards[i]->key < key)//这意味着KeyType类型必须有小于运算符“<”，若没有
        {                                                          //或者其表现与所需不符合，需要自己重载该运算符
            x = x->forwards[i];
        }
        insertPos.push_back(x);
    }

    x = x->forwards[0];
    if(x != NULL && x->key == key)//已存在该key值的节点，返回false
    {
        return false;
    }

    ListNode<KeyType, DataType> * newNode = new ListNode<KeyType, DataType>(key, value);
    int nodeLevel = randomLevel();
    reverse(insertPos.begin(), insertPos.end());//这种方式不好，之后优化对比下
    if(nodeLevel > level_num && level_num < MAX_LEVEL)
    {
        nodeLevel = ++level_num;
        head.forwards.push_back(NULL);
        insertPos.push_back(&head);
    }

    for(int i = 0; i <= nodeLevel; i++)
    {
        newNode->forwards.push_back(insertPos[i]->forwards[i]);
        insertPos[i]->forwards[i] = newNode;
    }

    return true;
}

template <class KeyType, class DataType>
bool SkipList<KeyType, DataType>::deleteNode(const KeyType & key)
{
    ListNode<KeyType, DataType> * x = &head;
    vector<ListNode<KeyType, DataType>*> deletePos;
    for(int i = level_num; i >= 0; i--)
    {
        while (x->forwards[i] != NULL && x->forwards[i]->key < key)//这意味着KeyType类型必须有小于运算符“<”，若没有
        {                                                          //或者其表现与所需不符合，需要自己重载该运算符
            x = x->forwards[i];
        }
        deletePos.push_back(x);
    }

    reverse(deletePos.begin(), deletePos.end());//这种方式不好，之后优化对比下
    x = x->forwards[0];
    if(x != NULL && x->key == key)//存在该节点，删除掉
    {
        for(int i = 0; i < x->forwards.size(); i++)
        {
            deletePos[i]->forward[i] = x->forwards[i];
        }
        delete x;

        //如果最高几层没有节点，需要维护跳跃链表的层数，减小层数
        int n = 0;
        while(n < level_num)
        {
            if(head.forwards[level_num-n] == NULL)
                n++;
            else
                break;
        }

        while(n--)
        {
            head.forwards.pop_back();
            level_num--;
        }

        return true;
    }

    return false;

}

template <class KeyType, class DataType>
bool SkipList<KeyType, DataType>::updateNode(const KeyType & key, const DataType & newValue)
{
    ListNode<KeyType, DataType> * x = &head;
    for(int i = level_num; i >= 0; i--)
    {
        while (x->forwards[i] != NULL && x->forwards[i]->key < key)//这意味着KeyType类型必须有小于运算符“<”，若没有
        {                                                          //或者其表现与所需不符合，需要自己重载该运算符
            x = x->forwards[i];
        }
    }

    x = x->forwards[0];
    if(x != NULL && x->key == key)//存在该节点，修改其值
    {
        x->value = newValue;
        return true;
    }
    else
    {
        return false;
    }
}

//按照预定的概率生成一个节点的随机层数
template <class KeyType, class DataType>
int SkipList<KeyType, DataType>::randomLevel()
{
    srand((unsigned int)time(NULL));
    int level = 1;
    while ((rand()&0xFFFF) < (unsigned short)(SKIP_LIST_P * (float)0xFFFF))
        level++;
    return  (level > MAX_LEVEL ? MAX_LEVEL : level);
}

#if defined(DEBUG)
//打印某一层跳跃链表的各个节点的值，仅用于调试
template <class KeyType, class DataType>
void SkipList<KeyType, DataType>::displayByLevel(int n)
{
    ListNode<KeyType, DataType> * p = head.forwards[n];
    while (p != NULL)
    {
        cout << "Key:" << p->key << " Value" << p->value << "  ";
        p = p->forwards[n];
    }
    cout << endl;
}

#endif

#endif //SOME_DATA_STRUCTRUE_SKIP_LIST_H_H

