#ifndef __CITIES_H
#define __CITIES_H

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
extern void printCity(const City& city);
extern void printCities(const Cities* cities, int maxCount = 0);

#endif // __CITIES_H
