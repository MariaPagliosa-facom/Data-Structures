// DESCRIPTION: Cities.h
// Type defintions and function headers for City and collections
//
// Author: Maria Luiza Pagliosa
// Last revision: 10/05/2024

#ifndef __CITIES_H
#define __CITIES_H

#include "AABB.h"

inline constexpr auto maxName = 32;
inline constexpr auto maxCityCount = 5570;

struct City
{
  int ibge_code;
  char city_name[maxName];
  float latitude;
  float longitude;
  int capital;
  int state_code;
  int siafi_id;
  int ddd;
  char time_zone[maxName];

}; // City

inline auto
location(const City& city)
{
  return Point2{city.latitude, city.longitude};
}

struct Cities
{
  City* data;
  int capacity;
  int count;

}; // Cities

inline void
deleteCities(Cities*& cities)
{
  delete []cities->data;
  delete cities;
  cities = nullptr;
}

extern Cities* readCitiesFromCSV(const char* filename);
extern void printCity(const City& city, void* = nullptr);
extern void printCity(const City& city, bool shortPrint);
extern void printCities(const Cities* cities, int maxCount = 0);

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
  CityPtrs cptrs;

  cptrs.data = new const City*[cptrs.capacity = size];
  cptrs.count = 0;
  return cptrs;
}

inline auto
createCityPtrs(Cities* cities)
{
  auto cptrs = createCityPtrs(cities->count);

  for (int i = 0; i < cities->count; ++i)
    cptrs.data[i] = cities->data + i;
  cptrs.count = cities->count;
  return cptrs;
}

inline void
deleteCityPtrs(CityPtrs& cptrs)
{
  delete []cptrs.data;
  cptrs.data = nullptr;
}

using CityCmp = int(*)(const City*, const City*);

extern void sortCities(CityPtrs& cptrs, int offset, int count, int axis);

#endif // __CITIES_H
