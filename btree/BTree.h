//
// Created by NorSnow_ZJ on 2018/4/24.
//

#ifndef SOME_DATA_STRUCTRUE_BTREE_H
#define SOME_DATA_STRUCTRUE_BTREE_H

#include <vector>
#include <algorithm>
#include <climits>
#include <memory>
#include <cassert>

#define DEFALUT_OUT_DEGREE  1000

template <class Key>
class BTreeKey
{
public:
    explicit BTreeKey(Key key) : key_(key), isMinKey_(false)
    {

    }

    explicit BTreeKey(bool isMinKey) : isMinKey_(isMinKey)
    {

    }

    bool isMinKey()
    {
        return isMinKey_;
    }

    bool operator<(BTreeKey<Key> & rhs)
    {
        if (isMinKey())
            return false;
        return key_ < rhs.key_;
    }
    bool operator==(BTreeKey<Key> & rhs)
    {
        if (isMinKey() && rhs.isMinKey())
            return true;
        return key_ == rhs.key_;
    }

private:
    bool isMinKey_;
    Key key_;
};

template <class KeyType>
class BaseNode
{
public:
    explicit BaseNode(bool isLeaf) : isLeaf_(isLeaf)
    {

    }

    bool isLeafNode()
    {
        return isLeaf_;
    }

protected:
    std::vector<KeyType> keys;
    bool isLeaf_;
};

template <class KeyType, class ValueType>
class Node : public BaseNode<KeyType>
{
public:
    using NodeIter = std::shared_ptr<BaseNode>;
    explicit Node(bool isLeaf) : BaseNode(isLeaf)
    {

    }

    bool get(const KeyType & key, ValueType & val)
    {
        assert(keys_.size() == values_.size());
        auto keyIter = std::find(keys_.begin(), keys_.end(), key);
        auto valIter = keyToValue(keyIter);

        if ( valIter == values_.end())
            return false;

        val = *valIter;
        return true;
    }

    bool getRange(const KeyType & keyLow, const KeyType & keyHigh, std::vector<ValueType> & res)
    {
        if (keyHigh < keyLow)
            return false;

        auto keyIterLow = std::lower_bound(keys_.begin(), keys_.end(), keyLow);
        auto valIterLow = keyToValue(keyIterLow);
        if (valIterLow == values_.end())
            return false;

        auto keyIterHigh = std::upper_bound(keys_.begin(), keys_.end(), keyHigh);
        auto valIterHigh = keyToValue(keyIterHigh);
        res.clear();
        std::copy(valIterLow, valIterHigh, res.begin());
        return true;
    }

    void insert(const KeyType& key, const ValueType & val)
    {
        auto keyIter = std::upper_bound(keys_.begin(), keys_.end(), key);
        keys_.insert(keyIter, key);
        auto valIter = keyToValue(keyIter);
        values_.insert(valIter, val);
    }

    bool erase(const KeyType & key)
    {
        auto  keyIter = std::find(keys_.begin(), keys_.end(), key);
        if (keyIter == keys_.end())
            return false;

        auto valIter = keyToValue(keyIter);
        keys_.erase(keyIter);
        values_.erase(valIter);
        return true;
    }

    NodeIter Next()
    {
        return next_;
    }

    NodeIter prev()
    {
        return prev_;
    }

private:
    std::vector<ValueType>::iterator keyToValue(std::vector<KeyType>::iterator keyIter)
    {
        assert(keys_.size() == values_.size());
        if (keyIter == keys_.end())
            return values_.end();

        size_t pos = keyIter - keys_.begin();
        return values_.begin() + pos;
    }

    std::vector<KeyType> keys_;
    std::vector<ValueType> values_;
    NodeIter prev_;
    NodeIter next_;
};

template <class BTreeKeyType, class BTreeValueType>
class Btree
{
public:
    using NodeIter = std::shared_ptr<BaseNode>;
    Btree() : root_(false)
    {
        //TODO:init root node
    }

    bool get(const BTreeKeyType & key, BTreeValueType & val)
    {
        //TODO:finish it
    }

    bool update(const BTreeKeyType & key, const BTreeValueType & val)
    {
        //TODO: finish it
    }

    void insert(const BTreeKeyType & key, const BTreeValueType & val)
    {
        //TODO:finish it
    }

    bool erase(const BTreeKeyType & key)
    {
        //TODO:finish it
    }

    bool getRange(const BTreeKeyType & keyLow, const BTreeKeyType & keyHigh, std::vector<BTreeValueType> & res)
    {
        //TODO:finish it
    }

private:
    Node<BTreeKeyType, NodeIter> root_;
};

#endif //SOME_DATA_STRUCTRUE_BTREE_H
