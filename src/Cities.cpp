#include "Cities.h"
#include <cstdio>
#include <cstring>

inline auto
createCities()
{
    auto cities = new Cities;

    cities->data = new City[maxCityCount];
    cities->capacity = cities->count = maxCityCount;
    return cities;
}

Cities*
readCitiesFromCSV(const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (file == nullptr)
        return nullptr;
    
    constexpr auto maxLine = 256; // max file line size
    char line[maxLine];
    auto cityCount = 0;

    // skip field names from CSV
    fgets(line, maxLine, file);

    auto cities = createCities();

    while (fgets(line, maxLine, file) != nullptr)
    {
        auto city = cities->data + cityCount++;

        sscanf(line,
            "%d,%[^,],%f,%f,%d,%d,%d,%d,%[^\n]",
            &city->ibge_code,
            city->city_name,
            &city->latitude,
            &city->longitude,
            &city->capital,
            &city->state_code,
            &city->siafi_id,
            &city->ddd,
            city->time_zone);
    }
    fclose(file);
    if (cityCount != maxCityCount)
    {
        deleteCities(cities);
        return nullptr;
    }
    return cities;
}

void
printCity(const City& city)
{
    printf("IBGE code: %d\n"
        "Name: %s\n"
        "Latitude: %f\n"
        "Longitude: %f\n"
        "Capital: %d\n"
        "State code : %d\n"
        "Siafi id: %d\n"
        "DDD: %d\n"
        "Time zone: %s\n",
        city.ibge_code,
        city.city_name,
        city.latitude,
        city.longitude,
        city.capital,
        city.state_code,
        city.siafi_id,
        city.ddd,
        city.time_zone);
}

void
printCities(const Cities* cities, int maxCount)
{
    if (cities == nullptr)
        return;
    if (maxCount <= 0 || maxCount > cities->count)
        maxCount = cities->count;
    for (int i = 0; i < maxCount; ++i)
        printCity(cities->data[i]);
}
