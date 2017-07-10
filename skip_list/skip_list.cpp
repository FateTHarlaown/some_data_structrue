//
// Created by 兔晓侠 on 2017/7/9.
//

#include <c++/cstdlib>
#include "skip_list.h"

SkipList::SkipList(int maxLevel, float skipListP):MAX_LEVEL(maxLevel),
                                                  SKIP_LIST_P(skipListP)
{
    level_num = 1;
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
    for(int i = level_num; i >= 0; i--)
    {
        while (x->forwards[i] != NULL && x->forwards[i]->key < key)//这意味着KeyType类型必须有小于运算符“<”，若没有
        {                                                          //或者其表现与所需不符合，需要自己重载该运算符
            x = x->forwards[i];
        }
    }

}
