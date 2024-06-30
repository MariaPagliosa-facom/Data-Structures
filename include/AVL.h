// DESCRIPTION: AVL.h
// Type defintions and function headers for generic AVL-tree
//
// Author: Maria Luiza Pagliosa
// Last revision: 28/06/2024

#ifndef __AVL_H
#define __AVL_H

#include "Cities.h"

struct AVLNode
{
  struct Data
  {
    const City* city;
    Data* next;

  }; // Data

  struct Interval
  {
    const City* min;
    const City* max;

  }; // Interval

  Data data;
  Interval interval;
  AVLNode* children[2];
  int height;

}; // AVLNode

inline auto
height(const AVLNode* node)
{
  return node == nullptr ? -1 : node->height;
}

extern void deleteAVLNode(AVLNode* node);

using DataCmp = int (*)(const City*, const City*);
using KeyCmp = int (*)(const City*, const void*);

struct AVLTree
{
  DataCmp dataCmp;
  KeyCmp keyCmp;
  AVLNode* root;
  int count;

}; // AVLTree

extern AVLTree* createAVLTree(Cities* cities, DataCmp dataCmp, KeyCmp keyCmp);

inline void
deleteAVLTree(AVLTree*& tree)
{
  deleteAVLNode(tree->root);
  delete tree;
  tree = nullptr;
}

inline auto
height(const AVLTree* tree)
{
  return height(tree->root);
}

using IterFunc = void (*)(const City&, void*);

extern const City* find(const AVLTree* tree, const void* key);
extern void findAll(const AVLTree* tree,
  const void* key,
  IterFunc func,
  void* params = nullptr);
extern void iterate(const AVLTree* tree,
  IterFunc func,
  void* params = nullptr,
  int maxCount = -1);
extern bool removeAll(AVLTree* tree, const void* key);
extern AVLTree* intersect(const AVLTree* a, const AVLTree* b);

struct Range
{
  const void* minKey;
  const void* maxKey;

}; // Range

inline auto
isNull(const Range& range)
{
  return range.minKey == nullptr;
}

extern void rangeQuery(const AVLTree* tree,
  const Range& range,
  IterFunc func,
  void* params = nullptr);
extern AVLTree* rangeQuery(const AVLTree* tree, const Range& range);

#endif // __AVL_H