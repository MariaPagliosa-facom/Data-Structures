// DESCRIPTION: AVL.cpp
// Source code for AVL-tree
//
// Author: Maria Luiza Pagliosa
// Last revision: 28/06/2024

#include "AVL.h"
#include <cassert>

inline auto
createAVLNode(const City* city)
{
  auto node = new AVLNode;

  node->data.city = city;
  node->data.next = nullptr;
  node->interval = {city, city};
  node->children[0] = node->children[1] = nullptr;
  node->height = 0;
  return node;
}

void
deleteAVLNode(AVLNode* node)
{
  if (node != nullptr)
  {
    deleteAVLNode(node->children[0]);
    deleteAVLNode(node->children[1]);
    while (auto sibiling = node->data.next)
    {
      node->data.next = sibiling->next;
      delete sibiling;
    }
    delete node;
  }
}

inline constexpr auto
max(int a, int b)
{
  return a > b ? a : b;
}

inline auto
maxHeight(const AVLNode* a, const AVLNode* b)
{
  return max(height(a), height(b));
}

void
rightRotation(AVLNode*& node)
{ 
  auto n = node;
  auto x = n->children[0];
  auto y = n->children[1];
  auto a = x->children[0];
  auto b = x->children[1];

  n->children[0] = b;
  x->children[1] = n;
  node = x;
  n->height = maxHeight(b, y) + 1;
  x->height = maxHeight(a, n) + 1;
}

void
leftRotation(AVLNode*& node)
{
  auto n = node;
  auto x = n->children[0];
  auto y = n->children[1];
  auto a = y->children[0];
  auto b = y->children[1];

  n->children[1] = a;
  y->children[0] = n;
  node = y;
  n->height = maxHeight(x, a) + 1;
  y->height = maxHeight(n, b) + 1;
}

void
rebalanceAVL(AVLNode*& node)
{
  switch (height(node->children[0]) - height(node->children[1]))
  {
    case -2:
    {
      auto& child = node->children[1];

      height(child->children[0]) - height(child->children[1]) <= 0 ?
        leftRotation(node) :
        (rightRotation(child), leftRotation(node));
      break;
    }
    case +2:
    {
      auto& child = node->children[0];

      height(child->children[0]) - height(child->children[1]) >= 0 ?
        rightRotation(node) :
        (leftRotation(child), rightRotation(node));
    }
  }
}

bool
isLeaf(AVLNode* node)
{
  return node->height == 0 ? true : false;
}

inline AVLNode*
downLeft(AVLNode* node)
{
  while (node->children[0] != nullptr)
    node = node->children[0];
  return node;
}

auto
remove(AVLNode*& node, const City* city, DataCmp dataCmp)
{
  if (node == nullptr)
    return false;

  auto cmp = dataCmp(node->data.city, city);

  if (cmp > 0)
    return remove(node->children[0], city, dataCmp);
  if (cmp < 0)
    return remove(node->children[1], city, dataCmp);
  if (isLeaf(node))
  {
    deleteAVLNode(node);
    node = nullptr;
    return true;
  }
  if (node->children[0] == nullptr || node->children[1] == nullptr)
  {
    auto temp = node;

    node = node->children[node->children[0] == nullptr];
    deleteAVLNode(temp);
    return true;
  }

  auto successor = downLeft(node->children[1]);
  auto temp = node;

  node = successor;
  return remove(temp->children[1], successor->data.city, dataCmp);
}

bool
removeAll(AVLTree* tree, const void* key)
{
  if (auto city = find(tree, key))
    return remove(tree->root, city, tree->dataCmp);
  return false;
}

auto
insert(AVLNode*& node, const City* city, DataCmp dataCmp)
{
  if (node == nullptr)
  {
    node = createAVLNode(city);
    return true;
  }

  auto cmp = dataCmp(city, node->data.city);

  if (cmp == 0)
  {
    auto sibiling = new AVLNode::Data;

    sibiling->city = city;
    sibiling->next = node->data.next;
    node->data.next = sibiling;
    return false;
  }

  auto& child = node->children[cmp > 0];
  auto ok = insert(child, city, dataCmp);

  node->height = maxHeight(node->children[0], node->children[1]) + 1;
  rebalanceAVL(node);
  return ok;
}

inline void
insert(AVLTree* tree, const City* city)
{
  if (insert(tree->root, city, tree->dataCmp))
    ++tree->count;
}

void
updateInterval(AVLNode* node)
{
  assert(node != nullptr);
  if (auto child = node->children[0])
  {
    updateInterval(child);
    node->interval.min = child->interval.min;
  }
  if (auto child = node->children[1])
  {
    updateInterval(child);
    node->interval.max = child->interval.max;
  }
}

inline AVLTree*
createAVLTree(DataCmp dataCmp, KeyCmp keyCmp)
{
  auto tree = new AVLTree;

  tree->dataCmp = dataCmp;
  tree->keyCmp = keyCmp;
  tree->root = nullptr;
  tree->count = 0;
  return tree;
}

AVLTree*
createAVLTree(Cities* cities, DataCmp dataCmp, KeyCmp keyCmp)
{
  auto tree = createAVLTree(dataCmp, keyCmp);

  for (auto i = 0; i < cities->count; ++i)
    insert(tree, cities->data + i);
  updateInterval(tree->root);
  return tree;
}

const City*
find(const AVLNode* node, const void* key, KeyCmp keyCmp)
{
  if (node == nullptr)
    return nullptr;

  auto cmp = keyCmp(node->data.city, key);

  return cmp == 0 ? node->data.city :
    find(node->children[cmp < 0], key, keyCmp);
}

const City*
find(const AVLTree* tree, const void* key)
{
  return find(tree->root, key, tree->keyCmp);
}

void
findAll(const AVLNode* node,
  const void* key,
  KeyCmp keyCmp,
  IterFunc func,
  void* params)
{
  if (node == nullptr)
    return;
  if (auto cmp = keyCmp(node->data.city, key); cmp == 0)
    for (auto data = &node->data; data != nullptr; data = data->next)
      func(*data->city, params);
  else
    findAll(node->children[cmp < 0], key, keyCmp, func, params);
}

void
findAll(const AVLTree* tree, const void* key, IterFunc func, void* params)
{
  return findAll(tree->root, key, tree->keyCmp, func, params);
}

int // returns the number of visited nodes
iterate(const AVLNode* node,
  IterFunc func,
  void* params,
  int count,
  int maxCount = -1)
{
  if (node == nullptr)
    return count;
  count = iterate(node->children[0], func, params, count, maxCount);
  for (auto data = &node->data; data != nullptr; data = data->next)
  {
    if (count == maxCount)
      return count;
    func(*data->city, params);
    ++count;
  }
  return iterate(node->children[1], func, params, count, maxCount);
}

void
iterate(const AVLTree* tree, IterFunc func, void* params, int maxCount)
{
  iterate(tree->root, func, params, 0, maxCount);
}

inline auto
contains(const Range& r, const AVLNode::Interval& i, KeyCmp keyComp)
{
  return keyComp(i.min, r.minKey) >= 0 && keyComp(i.max, r.maxKey) <= 0;
}

inline auto
disjoint(const Range& r, const AVLNode::Interval& i, KeyCmp keyComp)
{
  return keyComp(i.min, r.maxKey) > 0 || keyComp(i.max, r.minKey) < 0;
}

void
rangeQuery(const AVLNode* node,
  KeyCmp keyCmp,
  const Range& range,
  IterFunc func,
  void* params)
{
  if (node == nullptr || isNull(range))
    return;
  if (contains(range, node->interval, keyCmp))
    iterate(node, func, params, 0);
  else if (!disjoint(range, node->interval, keyCmp))
  {
    rangeQuery(node->children[0], keyCmp, range, func, params);
    rangeQuery(node->children[1], keyCmp, range, func, params);
  }
}

void
rangeQuery(const AVLTree* tree,
  const Range& range,
  IterFunc func,
  void* params)
{
  rangeQuery(tree->root, tree->keyCmp, range, func, params);
}

int
cmpDataByPtr(const City* city1, const City* city2)
{
  return int(size_t(city1) - size_t(city2));
}

int
cmpByPtr(const City* city, const void* key)
{
  return int(size_t(city) - size_t(key));
}

void
addCity(const City& city, void* params)
{
  insert((AVLTree*)params, &city);
}

AVLTree*
rangeQuery(const AVLTree* tree, const Range& range)
{
  AVLTree* out = createAVLTree(cmpDataByPtr, cmpByPtr);

  rangeQuery(tree, range, addCity, out);
  if (out->root == nullptr)
    deleteAVLTree(out);
  return out;
}

struct IntersectionParams
{
  AVLTree* out;
  const AVLTree* b;

}; // IntersectionParams

void
intersectionHandler(const City& city, void* params)
{
  if (find(((IntersectionParams*)params)->b, &city))
    insert(((IntersectionParams*)params)->out, &city);
}

AVLTree*
intersect(const AVLTree* a, const AVLTree* b)
{
  if (a == nullptr || b == nullptr)
    return nullptr;
  assert(a->dataCmp == b->dataCmp && a->keyCmp == b->keyCmp);
  assert(a->dataCmp == cmpDataByPtr && a->keyCmp == cmpByPtr);

  AVLTree* out = createAVLTree(cmpDataByPtr, cmpByPtr);
  IntersectionParams params{out, b};

  iterate(a, intersectionHandler, &params);
  if (out->root == nullptr)
    deleteAVLTree(out);
  return out;
}
