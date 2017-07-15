//
// Created by 兔晓侠 on 2017/7/9.
//

#include <c++/cstdlib>
#include <algorithm>
#include "skip_list.h"

SkipList::SkipList(int maxLevel, float skipListP):MAX_LEVEL(maxLevel),
                                                  SKIP_LIST_P(skipListP)
{
    level_num = 0;
    for(int i = 0; i < MAX_LEVEL; i++)
    {
        head.forwards[i] = NULL;
    }
}

template <class KeyType, class DataType>
bool SkipList<KeyType, DataType>::searchNode(const KeyType & key, DataType & value)
{
    ListNode * x = &head;
    for(int i = level_num; i >= 0; i--)
    {
        while (x->forwards[i] != NULL && x->forwards[i]->key < key)//这意味着KeyType类型必须有小于运算符“<”，若没有
        {                                                          //或者其表现与所需不符合，需要自己重载该运算符
            x = x->forwards[i];
        }
    }
    x = x->forwards[0];
    if(x != NULL && x->key == key)
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
    ListNode * x = &head;
    vector<ListNode*> insertPos;
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

    ListNode * newNode = new ListNode(key, value);
    int nodeLevel = randomLevel();
    reverse(insertPos.begin(), insertPos.end());
    if(nodeLevel > level_num && level_num < MAX_LEVEL)
    {
        nodeLevel = ++level_num;
        head.forwards.push_back(NULL);
        insertPos.push_back(&head);
    }

    for(int i = 0; i < level_num - nodeLevel; i++)
        insertPos.pop_back();

    for(int i = 0; i <= nodeLevel; i++)
    {
        newNode->forwards[i] = insertPos[i]->forwards[i];
        insertPos[i]->forwards[i] = newNode;
    }

    return true;
}
