// DESCRIPTION: KDTree.h
// Type defintions and function headers for cities KD-tree
//
// Author: Maria Luiza Pagliosa
// Last revision: 10/05/2024

#ifndef __KDTREE_H
#define __KDTREE_H

#include "KNN.h"

struct KDTreeNode
{
  int id;
  Box2 bounds;
  const City* midCity;
  KDTreeNode* children[2];
  int depth;
  int first;
  int count;

}; // KDTreeNode

inline KDTreeNode*
createKDTreeLeaf(int id, const Box2& bounds, int depth, int first, int count)
{
  auto node = new KDTreeNode;

  node->id = id;
  node->bounds = bounds;
  node->midCity = nullptr;
  node->children[0] = node->children[1] = nullptr;
  node->depth = depth;
  node->first = first;
  node->count = count;
  return node;
}

inline bool
isLeaf(const KDTreeNode* node)
{
  return node->midCity == nullptr;
}

extern void deleteKDTreeNode(KDTreeNode* node);

struct KDTree
{
  CityPtrs cptrs;
  int maxCitiesPerNode;
  KDTreeNode* root;
  int nodeCount;
  int leafCount;

}; // KDTree

extern KDTree* createKDTree(Cities* cities, int maxCitiesPerNode = 50);

inline void
deleteKDTree(KDTree*& tree)
{
  deleteCityPtrs(tree->cptrs);
  deleteKDTreeNode(tree->root);
  delete tree;
  tree = nullptr;
}

extern KNN findKNN(const KDTree* tree, const Point2& p, int k);

#endif // __KDTREE_H
