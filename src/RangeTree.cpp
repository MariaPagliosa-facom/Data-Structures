// DESCRIPTION: RangeTree.cpp
// Source code for range tree
//
// Author: Maria Luiza Pagliosa
// Last revision: 28/06/2024

#include "RangeTree.h"
#include <cstring>

int
cmpDataByName(const City* city1, const City* city2)
{
  return strcmp(city1->city_name, city2->city_name);
}

int
cmpByName(const City* city, const void* key)
{
  return strcmp(city->city_name, (const char*)key);
}

int
cmpDataByLatitude(const City* city1, const City* city2)
{
  return int(city1->latitude - city2->latitude);
}

int
cmpByLatitude(const City* city, const void* key)
{
  return int(city->latitude - toFloat(key));
}

int
cmpDataByLongitude(const City* city1, const City* city2)
{
  return int(city1->longitude - city2->longitude);
}

int
cmpByLongitude(const City* city, const void* key)
{
  return int(city->longitude - toFloat(key));
}

int
cmpDataByStateCode(const City* city1, const City* city2)
{
  return city1->state_code - city2->state_code;
}

int
cmpByStateCode(const City* city, const void* key)
{
  return city->state_code - toInt(key);
}

int
cmpDataByDDD(const City* city1, const City* city2)
{
  return city1->ddd - city2->ddd;
}

int
cmpByDDD(const City* city, const void* key)
{
  return city->ddd - toInt(key);
}

RangeTree
createRangeTree(Cities* cities)
{
  RangeTree rt;

  rt.trees[0] = createAVLTree(cities, cmpDataByName, cmpByName);
  rt.trees[1] = createAVLTree(cities, cmpDataByLatitude, cmpByLatitude);
  rt.trees[2] = createAVLTree(cities, cmpDataByLongitude, cmpByLongitude);
  rt.trees[3] = createAVLTree(cities, cmpDataByStateCode, cmpByStateCode);
  rt.trees[4] = createAVLTree(cities, cmpDataByDDD, cmpByDDD);
  return rt;
}

inline AVLTree*
rangeNQuery(const AVLTree* trees[], const Range ranges[], int n)
{
  AVLTree* out = nullptr; // this the result

  // For each non-null range in ranges, do 1.
  for (int i = 0; i < n; ++i)
    if (!isNull(ranges[i]))
      // 1: Run the range query on the corresponding tree.
      // If the result of the query is non-null, do 2.
      if (auto b = rangeQuery(trees[i], ranges[i]))
        // 2: If there is not a current result, it is the
        // result of the last query.
        if (out == nullptr)
          out = b;
        // Otherwise, it is the intersection of the
        // current result with the result of the last query.
        else
        {
          auto temp = intersect(out, b);

          deleteAVLTree(b);
          deleteAVLTree(out);
          out = temp;
        }
  return out;
}

AVLTree*
rangeQuery(const RangeTree& rt, const Range ranges[5])
{
  return rangeNQuery((const AVLTree**)rt.trees, ranges, 5);
}
