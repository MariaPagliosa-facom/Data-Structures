// DESCRIPTION: KNN.cpp
// Source code for KNN
//
// Author: Maria Luiza Pagliosa
// Last revision: 10/05/2024

#include <KNN.h>

inline bool
add(KNN& knn, float distance, const City* city)
{
  // Cannot insert entry into knn.data: too far away
  if (distance >= knn.data[knn.k].distance)
    return false;

  int i;
 
  // Lookup entry index according to its distance
  for (i = knn.count; i > 0; --i)
  {
    if (knn.data[i - 1].distance > distance)
      knn.data[i] = knn.data[i - 1];
    else
      break;
  }
  knn.data[i].distance = distance;
  knn.data[i].city = city;
  if (knn.count < knn.k)
    ++knn.count;
  return true;
}

bool
add(KNN& knn, const Point2& p, const City* city)
{
  return add(knn, distance(p, location(*city)), city);
}
