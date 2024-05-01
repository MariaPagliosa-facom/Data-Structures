#include "DB.h"
#include <cstdio>

int
main()
{
    auto cities = readCitiesFromCSV("data/municipios.csv");
    auto db = createDB(cities);

    printCities(cities, 10);
    deleteDB(db);
    return 0;
}