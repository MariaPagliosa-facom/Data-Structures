#ifndef __DB_H
#define __DB_H

#include "KDTree.h"

struct Hash
{
  const City** cityMap;
  int size;
  int collisionCount;

}; // Hash

struct DB
{
  Cities* cities;
  Hash hash1;
  Hash hash2;
  KDTree* tree;

}; // DB

extern DB* createDB(Cities* cities);

inline void
deleteDB(DB*& db)
{
  delete []db->hash1.cityMap;
  delete []db->hash2.cityMap;
  deleteKDTree(db->tree);
  deleteCities(db->cities);
  delete db;
  db = nullptr;
}

extern const City* findCity(const DB* db, int ibge_code);
extern CityPtrs findCities(const DB* db, const char* city_name);

#endif // __DB_H