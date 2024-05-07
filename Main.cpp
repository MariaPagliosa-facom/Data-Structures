#include "DB.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

bool
stoi(const char* s, int& value)
{
    char* end = nullptr;
    auto x = strtol(s, &end, 10);

    return *end == 0 || *end == '\n' ? (value = int(x)), true : false;
}

int
readInt(const char* prompt)
{
    constexpr auto maxInputSize = 80;
    char input[maxInputSize];

    for (;;)
    {
        printf("%s: ", prompt);
        fgets(input, maxInputSize, stdin);
        if (*input == '\n')
            return -1;

        int value;

        if (stoi(input, value))
            return value;
        puts("Integer expected! Try again.");
    }
}

void
printCity(const DB* db, int ibge_code)
{
    if (auto city = findCity(db, ibge_code))
        printCity(*city);
    else 
        printf("City with IBGE code %d not found.\n", ibge_code);
}

void
printKNN(const DB* db, int ibge_code)
{
    //TODO: task 2
    printf("KNN for %d\n", ibge_code);
}

const char*
readCityName()
{
    constexpr auto maxInputSize = 80;
    // global lifetime, local visibility
    static char input[maxInputSize];

    printf("Enter a city name: ");
    fgets(input, maxInputSize, stdin);
    if (*input == '\n')
        return nullptr;
    input[strlen(input) - 1] = 0;
    return input;
}

void
printKNN(const DB* db, const char* city_name)
{
    auto cities = findCities(db, city_name);

    if (cities.count == 0)
    {
        printf("City '%s' not found\n", city_name);
        return;
    }
    if (cities.count == 1)
        printKNN(db, cities.data[0]->ibge_code); // call task 2
    else
    {
        for (int i = 0; i < cities.count; ++i)
        {
            auto cPtr = cities.data[i];
            
            printf("% 3d %d %s %d\n",
                i + 1,
                cPtr->ibge_code,
                cPtr->city_name,
                cPtr->state_code);
        }
        for (;;)
        {
            auto i = readInt("Enter a city index");

            if (i > 0 && i <= cities.count)
            {
                printKNN(db, cities.data[i - 1]->ibge_code); // call task 2
                break;
            }
        }
    }
    deleteCityPtrs(cities);   
}

inline auto 
readIbgeCode()
{
    return readInt("Enter a city IBGE code (must be an integer)");
}

void
talktome(const DB* db)
{   
    for (;;)
    {
        printf("\nMENU\n"
           "[1] Get city info\n"
           "[2] Get neighbors' IBGE\n"
           "[3] Get neighbors' info\n"
           "[4] Exit\n"
           "Enter an option: ");

        constexpr auto maxInputSize = 80;
        char input[maxInputSize];
        char op{};

        fgets(input, maxInputSize, stdin);
        if (input[0] == '\n')
            continue;
        if (input[1] == '\n')
            op = *input;
        switch (op)
        {
        case '1':
            if (auto ibge_code = readIbgeCode(); ibge_code != -1)
                printCity(db, ibge_code);
            break;
        case '2':
            break;
        case '3':
            if (auto city_name = readCityName())
                printKNN(db, city_name);
            break;
        case '4':
            puts("Bye!");
            return;
        default:
            puts("Unknown option! Try again.\n");
            break;
        }
    }
}

int
main(int argc, char** argv)
{
    if (argc == 1)
    {
        puts("Usage:\nTest <city_file>");
        return 1;
    }
    
    auto cities = readCitiesFromCSV(argv[1]);
    auto db = createDB(cities);

    talktome(db);
    //printCities(cities, 10);
    deleteDB(db);
    return 0;
}