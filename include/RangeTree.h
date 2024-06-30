// DESCRIPTION: RangeTree.h
// Type defintions and function headers for range tree
//
// Author: Maria Luiza Pagliosa
// Last revision: 28/06/2024

#ifndef __RANGE_TREE_H
#define __RANGE_TREE_H

#include "AVL.h"

enum RangeType
{
  NAME,
  LATITUDE,
  LONGITUDE,
  STATE_CODE,
  DDD
};

struct RangeTree
{
  AVLTree* trees[5];

};  // RangeTree

extern RangeTree createRangeTree(Cities* cities);

inline void deleteRangeTree(RangeTree& rt)
{
  deleteAVLTree(rt.trees[NAME]);
  deleteAVLTree(rt.trees[LATITUDE]);
  deleteAVLTree(rt.trees[LONGITUDE]);
  deleteAVLTree(rt.trees[STATE_CODE]);
  deleteAVLTree(rt.trees[DDD]);
}

extern AVLTree* rangeQuery(const RangeTree& rt, const Range ranges[]);

inline auto
toInt(const void* x)
{
  return (int)((size_t)x);
}

inline auto
toVoidPtr(int x)
{
  return (const char*)((size_t)x);
}

inline auto
toFloat(const void* x)
{
  return (float)((size_t)x);
}

inline auto
toVoidPtr(float x)
{
  return (const char*)((size_t)x);
}

#endif // !__RANGE_TREE_H
