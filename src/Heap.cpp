// DESCRIPTION: Cities.cpp
// Source code for Heap
//
// Author: Maria Luiza Pagliosa
// Last revision: 10/05/2024

#include "Cities.h"
#include "DB.h"

inline int
parent(int nid)
{
  return nid == 0 ? 0 : (nid - 1) >> 1;
}

inline int
rChild(int nid)
{
  return (nid << 1) + 2;
}

inline int
lChild(int nid)
{
  return (nid << 1) + 1;
}

inline void
swap(const City*& a, const City*& b)
{
  auto aux = a;

  a = b;
  b = aux;
}

void
heapify(const City** data, int nid, int count, CityCmp cmp)
{
  int max = nid;

  if (auto rc = rChild(nid); rc < count && cmp(data[rc], data[nid]) > 0)
    max = rc;
  if (auto lc = lChild(nid); lc < count && cmp(data[lc], data[max]) > 0)
    max = lc;
  if (max != nid)
  {
    swap(data[max], data[nid]);
    heapify(data, max, count, cmp);
  }
}

inline void
buildHeap(const City** data, int count, CityCmp cmp)
{
  for (auto p = parent(count - 1); p >= 0;)
    heapify(data, p--, count, cmp);
}

void
heapSort(const City** data, int count, CityCmp cmp)
{
  buildHeap(data, count, cmp);
  while (count > 0)
  {
    swap(data[0], data[--count]);
    heapify(data, 0, count, cmp);
  }
}
