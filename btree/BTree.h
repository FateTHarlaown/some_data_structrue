//
// Created by NorSnow_ZJ on 2018/4/24.
//

#ifndef SOME_DATA_STRUCTRUE_BTREE_H
#define SOME_DATA_STRUCTRUE_BTREE_H

#include <vector>
#include <algorithm>
#include <climits>

#define DEFALUT_OU_DEGREE  1000

template <class Key>
class BTreeKey
{
public:
    BTreeKey(Key key, bool isMinKey = false) : key_(key), isMinKey_(isMinKey)
    {

    }

    bool isMinKey()
    {
        return isMinKey_;
    }

    bool operator<(BTreeKey<Key> & rhs)
    {
        return key_ < rhs.key_;
    }
    bool operator==(BTreeKey<Key> & rhs)
    {
        return key_ == rhs.key_;
    }

private:
    bool isMinKey_;
    Key key_;
};

template <class BtreeKeyType, class BtreeValueType>
class Node
{
public:
    using Element = std::pair<BtreeKeyType, BtreeValueType>;
    using Slots = std::vector<Element>;
    using SlotsIter = Slots::iterator;

    Node(bool isLeaf = false) : isLeaf_(isLeaf)
    {

    }

    bool get(const BtreeKeyType & key, BtreeValueType & val)
    {
        SlotsIter it = std::find_if(slots_.begin(), slots_.end(), [key](Element e)->bool { return key == e.first; });

        if (it == slots_.end())
        {
            return false;
        }

        val = it->second;
        return true;
    }

    bool getRange(const BtreeKeyType & keyLow, const BtreeKeyType & keyHigh)
    {
        //TODO：finish it
    }

    bool insert(const BtreeKeyType & key, const BtreeValueType & val)
    {
        //TODO:finish it
    }

    bool erase(const BtreeKeyType & key)
    {
        //TODO:finish it
    }

private:
    bool isLeaf_;
    Slots slots_;
};


#endif //SOME_DATA_STRUCTRUE_BTREE_H
