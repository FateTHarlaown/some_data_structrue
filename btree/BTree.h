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

    bool isMinKey() const
    {
        return isMinKey_;
    }

    bool operator<(const BTreeKey<Key> & rhs) const
    {
        if (isMinKey())
            return false;
        return key_ < rhs.key_;
    }
    bool operator==(const BTreeKey<Key> & rhs) const
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

    virtual ~BaseNode()
    {

    }

protected:
    std::vector<KeyType> keys;
    bool isLeaf_;
};

template <class KeyType, class ValueType>
class Node : public BaseNode<KeyType>
{
public:
    using NodeIter = std::shared_ptr<BaseNode<KeyType>>;
    explicit Node(bool isLeaf) : BaseNode<KeyType>(isLeaf)
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

    bool searchPos(const KeyType & key, ValueType & val)
    {
        if (keys_.empty())//这个节点是第一个叶子节点，并且还没有数据
            return false;

        auto keyPos = std::upper_bound(keys_.begin(), keys_.end(), key);
        keyPos--;
        auto valPos = keyToValue(keyPos);
        val = (*valPos);
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
        auto valIter = keyToValue(keyIter);
        keys_.insert(keyIter, key);
        values_.insert(valIter, val);
    }

    bool erase(const KeyType & key)
    {
        auto keyIter = std::find(keys_.begin(), keys_.end(), key);
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
    typename std::vector<ValueType>::iterator keyToValue(typename std::vector<KeyType>::iterator keyIter)
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
    using Key = BTreeKey<BTreeKeyType>;
    using BNode = BaseNode<Key>;
    using NodeIter = std::shared_ptr<BaseNode<Key>>;
    using BranchNode = Node<Key, NodeIter>;
    using LeafNode = Node<Key, BTreeValueType>;
    using BranchIter = std::shared_ptr<Node<Key, NodeIter>>;
    using LeafIter = std::shared_ptr<Node<Key, BTreeValueType>>;

    Btree() : root_(false)
    {
        assert(DEFALUT_OUT_DEGREE > 1);
    }

    void init()
    {
        LeafIter child(new LeafNode(true));
        root_.insert(Key(true), std::dynamic_pointer_cast<BNode>(child));
    }

    bool isInited()
    {
        return isInited_;
    }

    bool get(const BTreeKeyType & key, BTreeValueType & val)
    {
        Key findKey(key);
        NodeIter it;
        bool ret = root_.searchPos(findKey, it);
        assert(ret);//初始化后的根节点至少有一个min的子节点，不会为空

        while (!it->isLeafNode())
        {
            BranchIter tmp = std::dynamic_pointer_cast<BranchNode>(it);
            assert(tmp);
            if (tmp->searchPos(findKey, it))
                return false;
        }

        LeafIter leaf= std::dynamic_pointer_cast<LeafNode>(it);
        assert(leaf);
        ret = leaf->get(findKey, val);
        return ret;
    }

    bool update(const BTreeKeyType & key, const BTreeValueType & val)
    {
        //TODO: finish it
    }

    void insert(const BTreeKeyType & key, const BTreeValueType & val)
    {
        //TODO:finish it
        //test
        Key findKey(key);
        NodeIter it;
        bool flag = root_.searchPos(findKey, it);
        assert(flag);
        LeafIter leaf = std::dynamic_pointer_cast<LeafNode>(it);
        leaf->insert(findKey, val);
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
    bool isInited_;
    Node<Key, NodeIter> root_;
};

#endif //SOME_DATA_STRUCTRUE_BTREE_H
