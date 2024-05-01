#include "DB.h"

inline constexpr auto maxDBSize = 8009;

int
hash1(int key)
{
    return 0;
}

int
hash2(int key)
{
    return 1;
}

inline void
insertCity(DB* db, const City* city)
{
    auto h1 = hash1(city->ibge_code);
    auto h2 = hash2(city->ibge_code);

    for (int i = 0;; ++i, ++db->collisionCount)
    {
        auto k = (h1 + i * h2) % maxDBSize;

        if (db->cityMap[k] == nullptr)
        {
            db->cityMap[k] = city;
            db->size++;
            return;
        }
    }
}

DB*
createDB(Cities* cities)
{
    if (cities == nullptr)
        return nullptr;

    auto db = new DB;

    db->cities = cities;
    db->cityMap = new const City*[maxDBSize];
    db->size = db->collisionCount = 0;
    for (int i = 0; i < db->size; ++i)
        db->cityMap[i] = nullptr;
    for (int i = 0; i < cities->count; ++i)
        insertCity(db, cities->data + i);
    return db;
}

const City*
findCity(const DB* db, int ibge_code)
{
    auto h1 = hash1(ibge_code);
    auto h2 = hash2(ibge_code);

    for (int i = 0;; ++i)
    {
        auto k = (h1 + i * h2) % maxDBSize;
        auto city = db->cityMap[k];

        if (city == nullptr)
            return nullptr;
        if (city->ibge_code == ibge_code)
            return city;
    }
}
