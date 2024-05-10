// DESCRIPTION: KNN.h
// Type defintions and function headers for cities KNN
//
// Author: Maria Luiza Pagliosa
// Last revision: 10/05/2024

#ifndef __KNN_h
#define __KNN_h

#include "Cities.h"

struct KNN
{
  struct Entry
  {
    float distance;
    const City* city;

  }; // Entry

  int k; // capacity
  Entry* data;
  int count;

}; // KNN

inline KNN
createKNN(int k)
{
  KNN knn;

  knn.k = k;
  // Extra data entry for vector insertion
  knn.data = new KNN::Entry[k + 1];
  for (int i = 0; i <= k; ++i)
    knn.data[i].distance = FLT_MAX;
  knn.count = 0;
  return knn;
}

extern bool add(KNN& knn, const Point2& p, const City* city);

inline void
deleteKNN(KNN& knn)
{
  delete []knn.data;
}

inline auto
maxDistance(const KNN& knn)
{
  return knn.data[knn.count].distance;
}

#endif // __KNN_h
