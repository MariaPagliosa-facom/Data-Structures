#ifndef __DB_H
#define __DB_H

#include "Cities.h"

struct DB
{
    Cities* cities;
    const City** cityMap;
    int size;
    int collisionCount;

}; // DB

extern DB* createDB(Cities* cities);

inline void
deleteDB(DB*& db)
{
    deleteCities(db->cities);
    delete []db->cityMap;
    delete db;
    db = nullptr;
}

extern const City* findCity(const DB* db, int ibge_code); 

#endif // __DB_H