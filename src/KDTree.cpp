// DESCRIPTION: KDTree.cpp
// Source code for KDTree
//
// Author: Maria Luiza Pagliosa
// Last revision: 10/05/2024

#include "KDTree.h"
#include <cassert>
#include <cstdio>

// #define _DEBUG_TREE

void
deleteKDTreeNode(KDTreeNode* node)
{
  if (!isLeaf(node))
  {
    deleteKDTreeNode(node->children[0]);
    deleteKDTreeNode(node->children[1]);
  }
  delete node;
}

inline void
printb(const char* label, const Box2& bounds)
{
  printf("%smin<%f,%f> max<%f,%f>\n",
    label,
    bounds.min.x[0],
    bounds.min.x[1],
    bounds.max.x[0],
    bounds.max.x[1]);
}

void
split(KDTree* tree, KDTreeNode* node)
{
  assert(isLeaf(node));

  auto count = node->count;

#ifdef _DEBUG_TREE
  printf("**Splitting node %d\n", node->id);
  printb("Bounds: ", node->bounds);
  printf("Depth: %d\n"
    "First: %d\n"
    "Count: %d\n",
    node->depth,
    node->first,
    count);
#endif // _DEBUG_TREE

  // If the number of cities within the node does not exceed
  // the maximum allowed, then there is nothing to do
  if (count <= tree->maxCitiesPerNode)
  {
#ifdef _DEBUG_TREE
    puts("**Skipped\n");
#endif // _DEBUG_TREE
    return;
  }

  auto first = node->first;
  // The spliting direction depends on the node's depth
  // (x-axis for even depth and y-axis for odd depth)
  auto axis = node->depth & 1;

  // Sort the cities within the node
  sortCities(tree->cptrs, first, count, axis);

  // If the node's count is odd, add 1 to the count of the
  // second child
  auto odd = count & 1;
  // Get the "medium" city
  auto mid = first + (count >>= 1); // "medium" city index
  auto midCity = tree->cptrs.data[mid];

  // Set the node as a branch node
  node->midCity = midCity;
  node->first = node->count = 0;

  auto x = location(*midCity).x[axis];
  Box2 b0;
  Box2 b1;

#ifdef _DEBUG_TREE
  printf("Mid city x: %f (axis: %d)\n\n", x, axis);
#endif // _DEBUG_TREE
  // Split the node's AABB
  split(node->bounds, x, axis, b0, b1);

  // Create the two node's children...
  auto id = node->id << 1;
  auto c0 = createKDTreeLeaf(id + 1, b0, node->depth + 1, first, count);
  auto c1 = createKDTreeLeaf(id + 2, b1, node->depth + 1, mid, count + odd);

  node->children[0] = c0;
  node->children[1] = c1;
  tree->nodeCount += 2;
  tree->leafCount += 1;
  // ...and split them
  split(tree, c0);
  split(tree, c1);
}

KDTree*
createKDTree(Cities* cities, int maxCitiesPerNode)
{
  auto bounds = createEmptyBox2();

  for (int i = 0; i < cities->count; ++i)
    expand(bounds, location(cities->data[i]));

  auto tree = new KDTree;

  tree->cptrs = createCityPtrs(cities);
  tree->maxCitiesPerNode = maxCitiesPerNode;
  tree->root = createKDTreeLeaf(0, bounds, 0, 0, cities->count);
  tree->nodeCount = tree->leafCount = 1;
  split(tree, tree->root);
  return tree;
}

void
findKNN(const KDTree* tree, const KDTreeNode* node, const Point2& p, KNN& knn)
{
  // If node is leaf, test all cities within the node
  if (isLeaf(node))
  {
    for (int i = node->first, e = i + node->count; i < e; ++i)
      add(knn, p, tree->cptrs.data[i]);
    return;
  }

  auto c0 = node->children[0];
  auto c1 = node->children[1];
  auto d0 = distance(c0->bounds, p);
  auto d1 = distance(c1->bounds, p);

  if (d0 < d1)
  {
    if (d0 < maxDistance(knn))
    {
      findKNN(tree, c0, p, knn);
      if (d1 < maxDistance(knn))
        findKNN(tree, c1, p, knn);
    }
  }
  else if (d1 < maxDistance(knn))
  {
    findKNN(tree, c1, p, knn);
    if (d0 < maxDistance(knn))
      findKNN(tree, c0, p, knn);
  }
}

KNN
findKNN(const KDTree* tree, const Point2& p, int k)
{
  assert(k > 0);

  auto knn = createKNN(k);

  findKNN(tree, tree->root, p, knn);
  return knn;
}
