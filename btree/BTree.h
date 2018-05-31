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
#include "../../../../../../MinGW/lib/gcc/mingw32/4.8.1/include/c++/utility"

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
    bool isLeaf_;
};


template <class KeyType, class ValueType>
struct NodeIterator {
    NodeIterator(typename std::vector<KeyType>::iterator keyIter,
                 typename std::vector<ValueType>::iterator valIter) : key(keyIter), val(valIter)
    {}

    NodeIterator<KeyType, ValueType> operator++()
    {
        NodeIterator<KeyType, ValueType> ret(key, val);
        key++;
        val++;
        return ret;
    }

    NodeIterator<KeyType, ValueType> operator--()
    {
        NodeIterator<KeyType, ValueType> ret(key, val);
        key--;
        val--;
        return ret;
    }

    typename std::pair<KeyType, ValueType> operator*()
    {
        return std::pair<KeyType, ValueType>(*key, *val);
    }

    bool operator==(const NodeIterator<KeyType, ValueType> & rhs)
    {
        return (key == rhs.key) && (val == rhs.val);
    }

    bool operator!=(const NodeIterator<KeyType, ValueType> & rhs)
    {
        return (key != rhs.key) && (val != rhs.val);
    }

    typename std::vector<KeyType>::iterator key;
    typename std::vector<ValueType>::iterator val;
};

template <class KeyType, class ValueType>
class Node : public BaseNode<KeyType>
{
public:
    using NodeIter = std::shared_ptr<BaseNode<KeyType>>;
    using BranchIter = std::shared_ptr<Node<KeyType, NodeIter>>;
    using iterator = NodeIterator<KeyType, ValueType>;

    explicit Node(bool isLeaf) : BaseNode<KeyType>(isLeaf)
    {

    }

    size_t size()
    {
        return keys_.size();
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

    bool insert(const KeyType& key, const ValueType & val)
    {
        //已存在这个key则返回错误，key必须唯一
        if (std::find(keys_.begin(), keys_.end(), key) != keys_.end())
            return false;

        auto keyIter = std::upper_bound(keys_.begin(), keys_.end(), key);
        auto valIter = keyToValue(keyIter);
        keys_.insert(keyIter, key);
        values_.insert(valIter, val);
        return true;
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

    bool update(const KeyType & key, const ValueType & val)
    {
        auto keyIter = std::find(keys_.begin(), keys_.end(), key);
        if (keyIter == keys_.end())
            return false;

        auto valIter = keyToValue(keyIter);
        *valIter = val;
        return true;
    }

    void clear()
    {
        keys_.clear();
        values_.clear();
        parent_.reset();
        next_.reset();
        prev_.reset();
    }

    NodeIter next() const
    {
        return next_;
    }

    NodeIter prev() const
    {
        return prev_;
    }

    BranchIter parent() const
    {
        return parent_;
    }

    void setNext(const NodeIter & next)
    {
        next_ = next;
    }

    void setPrev(const NodeIter & prev)
    {
        prev_ = prev;
    }

    void setParent(const BranchIter & parent)
    {
        parent_ = parent;
    }

    iterator begin()
    {
        return iterator(keys_.begin(), values_.begin());
    }

    iterator end()
    {
        return iterator(keys_.end(), values_.end());
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
    BranchIter parent_;
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

    Btree()
    {
        assert(DEFALUT_OUT_DEGREE > 3);
    }

    void init()
    {
        root_ = std::make_shared<BranchNode>(false);
        LeafIter child = std::make_shared<LeafNode>(true);
        child->setParent(root_);
        root_->insert(Key(true), child);
        isInited_ = true;
    }

    bool isInited()
    {
        return isInited_;
    }

    bool get(const BTreeKeyType & key, BTreeValueType & val)
    {
        LeafIter leaf;
        if (!getLeafNode(key, leaf))
            return false;

        Key findKey(key);
        return leaf->get(findKey, val);
    }

    bool update(const BTreeKeyType & key, const BTreeValueType & val)
    {
        LeafIter leaf;
        if (!getLeafNode(key, leaf))
            return false;
        Key updateKey(key);
        return leaf->update(updateKey, val);
    }

    bool insert(const BTreeKeyType & key, const BTreeValueType & val)
    {
        LeafIter leaf;
        if (!getLeafNode(key, leaf))
            return false;

        Key insertKey(key);
        if (!leaf->insert(insertKey, val))
            return false;

        if (leaf->size() > DEFALUT_OUT_DEGREE)
        {
            LeafIter newLeaf = std::make_shared<LeafNode>(true);
            newLeaf->clear();
            newLeaf->setParent(leaf->parent());
            newLeaf->setNext(leaf->next());
            newLeaf->setPrev(leaf);
            leaf->setNext(newLeaf);
            LeafIter nextLeaf = std::dynamic_pointer_cast<LeafNode>(newLeaf->next());
            nextLeaf->setPrev(newLeaf);

            typename LeafNode::iterator it = leaf->begin();
            for (int i = 0; i < leaf->size()/2; i++)
                ++it;
            for ( ; it != leaf->end(); ++it)
            {
                newLeaf->insert((*it).first, (*it).second);
                leaf->erase((*it).first);
            }

            BranchIter parent = leaf->parent();
            auto firstKv = *leaf->begin();
            if (insertKey < firstKv.first)
            {
                parent->erase(firstKv.first);
                parent->insert(insertKey, std::dynamic_pointer_cast<BNode>(leaf));
            }

            firstKv = *newLeaf->begin();
            parent->insert(firstKv.first, std::dynamic_pointer_cast<BNode>(newLeaf));
        }

        BranchIter branch = leaf->parent();
        while (branch != root_ && branch->size() > DEFALUT_OUT_DEGREE)
        {
            BranchIter newBranch = std::make_shared<BranchNode>(false);
            newBranch->clear();
            newBranch->setParent(branch->parent());
            newBranch->setNext(branch->next());
            newBranch->setPrev(branch);
            branch->setNext(newBranch);
            BranchIter nextBranch = std::dynamic_pointer_cast<BranchNode>(newBranch->next());
            nextBranch->setPrev(newBranch);

            typename BranchNode::iterator it = branch->begin();
            for (int i = 0; i < branch->size()/2; ++i)
                ++it;
            for ( ; it != branch->end(); ++it)
            {
                newBranch->insert((*it).first, (*it).second);
                branch->erase((*it).first);
            }

            branch = branch->parent();
            auto firstKv = *newBranch->begin();
            branch->insert(firstKv.first, std::dynamic_pointer_cast<BNode>(newBranch));
        }

        if (branch == root_ && branch->size() == DEFALUT_OUT_DEGREE)
        {
            BranchIter branch1 = std::make_shared<BranchNode>(false);
            BranchIter branch2 = std::make_shared<BranchNode>(false);
            branch1->clear();
            branch2->clear();
            branch1->setParent(root_);
            branch2->setParent(root_);
            branch1->setNext(std::dynamic_pointer_cast<BNode>(branch2));
            branch2->setPrev(std::dynamic_pointer_cast<BNode>(branch1));

            typename BranchNode::iterator it = root_->begin();
            for (int i = 0; i < root_->size()/2; ++i)
            {
                branch1->insert((*it).first, (*it).second);
                ++it;
            }
            for ( ; it != root_->end(); ++it)
            {
               branch2->insert((*it).first, (*it).second);
            }
            root_->clear();
            Key minKey(true);
            root_->insert(minKey, std::dynamic_pointer_cast<BNode>(branch1));
            auto firstKv = *branch2->begin();
            root_->insert(firstKv.first, std::dynamic_pointer_cast<BNode>(branch2));
        }

        return true;
    }

    bool erase(const BTreeKeyType & key)
    {
        //TODO:finish it
        LeafIter leaf;
        if (!getLeafNode(key, leaf))
            return false;

        Key eraseKey(key);
        if (!leaf->erase(eraseKey))
            return false;

        BranchIter parent = leaf->parent();
        if (0 == leaf->size())
        {
            if (!parent->erase(eraseKey))
            {
                parent->erase(Key(true));
                if (parent->size() > 0)
                {
                    auto kv = *(parent->begin());
                    parent->erase(kv.first);
                    parent->insert(Key(true), std::dynamic_pointer_cast<BNode>(kv.second));
                }
                else
                {
                    eraseKey = Key(true);
                }
            }

            while (parent != root_ && parent->size() == 0)
            {
                BranchIter branch = parent;
                parent = parent->parent();
                if (parent->erase(eraseKey))
            }
        }
        else if (eraseKey < (*(leaf->begin())).first)
        {
            NodeIter tmp;
            if (parent->get(eraseKey, tmp))
            {
                parent->erase(eraseKey);
                parent->insert((*(leaf->begin())).first, std::dynamic_pointer_cast<NodeIter>(leaf));
            }
        }
    }

    bool getRange(const BTreeKeyType & keyLow, const BTreeKeyType & keyHigh, std::vector<BTreeValueType> & res)
    {
        //TODO:finish it
    }

private:
    bool getLeafNode(const BTreeKeyType & key, LeafIter & leaf)
    {
        Key findKey(key);
        NodeIter it;
        bool ret = root_->searchPos(findKey, it);
        assert(ret);//初始化后至少有一个min的子节点，不会为空

        while (!it->isLeafNode())
        {
            BranchIter tmp = std::dynamic_pointer_cast<BranchNode>(it);
            assert(tmp);
            if (tmp->searchPos(findKey, it))
                return false;
        }

        leaf = std::dynamic_pointer_cast<LeafNode>(it);
        return true;
    }

    bool isInited_;
    BranchIter root_;
};

#endif //SOME_DATA_STRUCTRUE_BTREE_H
