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
bool SkipList<KeyType, DataType>::searchNode(const KeyType & key, const DataType & value)
{

}
