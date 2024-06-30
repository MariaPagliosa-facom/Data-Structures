// DESCRIPTION: DB.cpp
// Source code for database
//
// Author: Maria Luiza Pagliosa
// Last revision: 09/06/2024

#include "DB.h"
#include <cstring>

inline constexpr auto maxDBSize = 10007;

inline int
hashFunc1(int key, int size)
{
  return key % size;
}

inline int
hashFunc2(int key, int size)
{
  return 1 + hashFunc1(key, size - 2);
}

inline int
hashFunc1(const char* key, int size)
{
  int ret = 0;

  for (int n = int(strlen(key)), i = 0; i < n; i++)
    ret += key[i];
  return ret % size;
}

inline int
hashFunc2(const char* key, int size)
{
  return 1 + hashFunc1(key, size - 2);
}

inline void
insertCity1(Hash& hash1, const City* city)
{
  auto h1 = hashFunc1(city->ibge_code, maxDBSize);
  auto h2 = hashFunc2(city->ibge_code, maxDBSize);

  for (int i = 0;; ++i, ++hash1.collisionCount)
  {
    auto k = (h1 + i * h2) % maxDBSize;

    if (hash1.cityMap[k] == nullptr)
    {
      hash1.cityMap[k] = city;
      hash1.size++;
      return;
    }
  }
}

inline void
insertCity2(Hash& hash2, const City* city)
{
  auto h1 = hashFunc1(city->city_name, maxDBSize);
  auto h2 = hashFunc2(city->city_name, maxDBSize);

  for (int i = 0;; ++i, ++hash2.collisionCount)
  {
    auto k = (h1 + i * h2) % maxDBSize;

    if (hash2.cityMap[k] == nullptr)
    {
      hash2.cityMap[k] = city;
      hash2.size++;
      return;
    }
  }
}

void
inicializeHash(Hash& hash)
{
  hash.cityMap = new const City*[maxDBSize];
  hash.size = hash.collisionCount = 0;
  for (int i = 0; i < maxDBSize; ++i)
    hash.cityMap[i] = nullptr;
}

DB*
createDB(Cities* cities)
{
  if (cities == nullptr)
    return nullptr;

  auto db = new DB;

  db->cities = cities;
  inicializeHash(db->hash1);
  inicializeHash(db->hash2);
  for (int i = 0; i < cities->count; ++i)
  {
    auto city = cities->data + i;

    insertCity1(db->hash1, city);
    insertCity2(db->hash2, city);
  }
  db->kdt = createKDTree(cities);
  db->rt = createRangeTree(cities);
  return db;
}

const City*
findCity(const DB* db, int ibge_code)
{
  auto h1 = hashFunc1(ibge_code, maxDBSize);
  auto h2 = hashFunc2(ibge_code, maxDBSize);

  for (int i = 0;; ++i)
  {
    auto k = (h1 + i * h2) % maxDBSize;
    auto city = db->hash1.cityMap[k];

    if (city == nullptr)
      return nullptr;
    if (city->ibge_code == ibge_code)
      return city;
  }
}

inline void
resize(CityPtrs& cptrs, int newSize)
{
  auto temp = cptrs.data;

  cptrs.data = new const City*[cptrs.capacity = newSize];
  for (int i = 0; i < cptrs.count; i++)
    cptrs.data[i] = temp[i];
  delete[]temp;
}

inline void
add(CityPtrs& cptrs, const City* city)
{
  constexpr auto deltaSize = 10;

  if (cptrs.count == cptrs.capacity)
    resize(cptrs, cptrs.count + deltaSize);
  cptrs.data[cptrs.count++] = city;
}

CityPtrs
findCities(const DB* db, const char* city_name)
{
  auto cptrs = createCityPtrs();
  auto h1 = hashFunc1(city_name, maxDBSize);
  auto h2 = hashFunc2(city_name, maxDBSize);

  for (int i = 0;; i++)
  {
    auto k = (h1 + i * h2) % maxDBSize;
    auto city = db->hash2.cityMap[k];

    if (city == nullptr)
      break;
    if (strcmp(city->city_name, city_name) == 0)
      add(cptrs, city);
  }
  return cptrs;
}
