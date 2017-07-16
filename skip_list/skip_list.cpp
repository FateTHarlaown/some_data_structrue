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
//无论如何至少有第0层，初始时第0层只有一个头结点
    head.forwards.push_back(NULL);
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
bool SkipList<KeyType, DataType>::deleteNode(KeyType key)
{
    ListNode * x = &head;
    vector<ListNode*> deletePos;
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
bool SkipList<KeyType, DataType>::updateNode(KeyType key, DataType newValue)
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