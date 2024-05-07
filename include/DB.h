#ifndef __DB_H
#define __DB_H

#include "Cities.h"

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

}; // DB

extern DB* createDB(Cities* cities);

inline void
deleteDB(DB*& db)
{
    deleteCities(db->cities);
    delete []db->hash1.cityMap;
    delete []db->hash2.cityMap;
    delete db;
    db = nullptr;
}

// Array of City pointers
struct CityPtrs
{
    const City** data;
    int capacity;
    int count;
    
}; // CityPtrs

inline auto
createCityPtrs(int size = 0)
{
    CityPtrs cities;

    cities.data = new const City*[cities.capacity = size];
    cities.count = 0;
    return cities;
}

inline void
deleteCityPtrs(CityPtrs& cities)
{
    delete []cities.data;
    cities.data = nullptr;
}

extern const City* findCity(const DB* db, int ibge_code); 
extern CityPtrs findCities(const DB* db, const char* city_name);

#endif // __DB_H