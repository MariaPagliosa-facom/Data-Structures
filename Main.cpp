#include "DB.h"
#include "Tests.h"
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

inline bool
stoi(const char* s, int& value)
{
  char* end = nullptr;
  auto x = strtol(s, &end, 10);

  return *end == 0 || *end == '\n' ? (value = int(x)), true : false;
}

int
readInt(const char* fmt, ...)
{
  constexpr auto maxInputSize = 80;
  char input[maxInputSize];
  char prompt[maxInputSize];
  va_list args;

  va_start(args, fmt);
  vsnprintf(prompt, maxInputSize, fmt, args);
  for (;;)
  {
    printf("%s: ", prompt);
    fgets(input, maxInputSize, stdin);
    if (*input == '\n')
      return -1;

    int value;

    if (stoi(input, value))
      return value;
    puts("Integer expected! Try again");
  }
}

inline bool
stof(const char* s, float& value)
{
  char* end = nullptr;
  auto x = strtof(s, &end);

  return *end == 0 || *end == '\n' ? (value = x), true : false;
}

float
readFloat(const char* fmt, ...)
{
  constexpr auto maxInputSize = 80;
  char input[maxInputSize];
  char prompt[maxInputSize];
  va_list args;

  va_start(args, fmt);
  vsnprintf(prompt, maxInputSize, fmt, args);
  for (;;)
  {
    printf("%s: ", prompt);
    fgets(input, maxInputSize, stdin);
    if (*input == '\n')
      return NAN;

    float value;

    if (stof(input, value))
      return value;
    puts("Float expected! Try again");
  }
}

void
printCity(const DB* db, int ibge_code)
{
  if (auto city = findCity(db, ibge_code))
    printCity(*city);
  else
    printf("City with IBGE code %d not found\n\n", ibge_code);
}

void
printKNN(const DB* db, const City* city, bool shortPrint = false)
{
  constexpr auto maxNeighbors = 50;

  for (;;)
  {
    auto k = readInt("Enter number of neighbors");

    if (k > 0 && k <= maxNeighbors)
    {
      auto p = location(*city);
      auto knn = findKNN(db->kdt, p, k + 1);

      printf("\n%d nearest neighbors of %s:\n", knn.count - 1, city->city_name);
      for (int i = 0; i < knn.count; i++)
        if (auto nc = knn.data[i].city; nc != city)
        {
          printf("%d (distance: %f)\n", i, knn.data[i].distance);
          printCity(*nc, shortPrint);
        }
      deleteKNN(knn);
      return;
    }
    printf("Number must be in range [1,%d]\n\n", maxNeighbors);
  }
}

void
printKNN(const DB* db, int ibge_code, bool shortPrint = false)
{
  if (auto city = findCity(db, ibge_code))
    printKNN(db, city, shortPrint);
  else
    printf("City with IBGE code %d not found\n\n", ibge_code);
}

const char*
readCityName(const char* label = "")
{
  constexpr auto maxInputSize = 80;
  // global lifetime, local visibility
  static char input[maxInputSize];

  printf("Enter a city name%s: ", label);
  fgets(input, maxInputSize, stdin);
  if (*input == '\n')
    return nullptr;
  input[strlen(input) - 1] = 0;
  return input;
}

void
printKNN(const DB* db, const char* city_name)
{
  auto cptrs = findCities(db, city_name);

  if (cptrs.count == 0)
  {
    printf("City '%s' not found\n\n", city_name);
    return;
  }
  if (cptrs.count == 1)
    printKNN(db, cptrs.data[0]->ibge_code); // call task 2
  else
  {
    for (int i = 0; i < cptrs.count; ++i)
    {
      auto cPtr = cptrs.data[i];

      printf("% 3d %d %s %d\n",
        i + 1,
        cPtr->ibge_code,
        cPtr->city_name,
        cPtr->state_code);
    }
    for (;;)
    {
      auto i = readInt("Enter a city index");

      if (i > 0 && i <= cptrs.count)
      {
        printKNN(db, cptrs.data[i - 1]);
        break;
      }
      printf("Number must be in range [1,%d]\n", cptrs.count);
    }
  }
  deleteCityPtrs(cptrs);
}

inline auto
readIbgeCode()
{
  return readInt("Enter a city IBGE code (must be an integer)");
}

inline void
printRangeQuery(const DB* db, const Range range[])
{
  puts("**Range query result**");
  if (auto out = rangeQuery(db->rt, range))
  {
    iterate(out, printCity);
    deleteAVLTree(out);
  }
  else
    puts("Empty!");
}

void
setNameRange(Range& range)
{
  constexpr auto maxNameSize = 40;
  static char  min[maxNameSize];

  strncpy(min, readCityName(" (min)"), maxNameSize);
  if (*min == 0)
  {
    range.minKey = nullptr;
    return;
  }
  range.minKey = min;
  range.maxKey = readCityName(" (max)");
  if (range.maxKey == nullptr)
    range.maxKey = range.minKey;
}

void
setIntRange(const char* label, Range& range)
{
  auto min = readInt("Enter %s (min)", label);

  if (min == -1)
  {
    range.minKey = nullptr;
    return;
  }
  range.minKey = toVoidPtr(min);

  auto max = readInt("Enter %s (max)", label);

  range.maxKey = max == -1 ? range.minKey : toVoidPtr(max);
}

void
setFloatRange(const char* label, Range& range)
{
  auto min = readFloat("Enter %s (min)", label);

  if (isnan(min))
  {
    range.minKey = nullptr;
    return;
  }
  range.minKey = toVoidPtr(min);

  auto max = readFloat("Enter %s (max)", label);

  range.maxKey = isnan(max) ? range.minKey : toVoidPtr(max);
}

void
rangeQueryInterface(const DB* db)
{
  Range ranges[]
  {
    {nullptr, nullptr}, // NAME
    {nullptr, nullptr}, // LATITUDE
    {nullptr, nullptr}, // LONGITUDE
    {nullptr, nullptr}, // STATE_CODE
    {nullptr, nullptr}  // DDD
  };

  for (;;)
  {
    printf("RANGE QUERY MENU\n"
      "[1] Set City Name Range\n"
      "[2] Set Latitude Range\n"
      "[3] Set Longitude Range\n"
      "[4] Set State Code Range\n"
      "[5] Set DDD Range\n"
      "[6] Run Query\n"
      "[7] Return to Main Manu\n"
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
        setNameRange(ranges[NAME]);
        break;
      case '2':
        setFloatRange("Latitude", ranges[LATITUDE]);
        break;
      case '3':
        setFloatRange("Longitude", ranges[LONGITUDE]);
        break;
      case '4':
        setIntRange("State Code", ranges[STATE_CODE]);
        break;
      case '5':
        setIntRange("DDD", ranges[DDD]);
        break;
      case '6':
        printRangeQuery(db, ranges);
        [[fallthrough]];
      case '7':
        return;
      default:
        puts("Unknown option! Try again\n");
        break;
    }
  }
}

void
talktome(const DB* db)
{
  for (;;)
  {
    printf("MENU\n"
      "[1] Get city info\n"
      "[2] Get neighbors' IBGE\n"
      "[3] Get neighbors' info\n"
      "[4] Range Query\n"
      "[5] Exit\n"
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
        if (auto ibge_code = readIbgeCode(); ibge_code != -1)
          printKNN(db, ibge_code, true);
        break;
      case '3':
        if (auto city_name = readCityName())
          printKNN(db, city_name);
        break;
      case '4':
        rangeQueryInterface(db);
        break;
      case '5':
        puts("Bye!");
        return;
      default:
        puts("Unknown option! Try again\n");
        break;
    }
  }
}

int
main(int argc, char** argv)
{
  //boxTest();
  if (argc == 1)
  {
    puts("Usage:\nT2 <city_file>");
    return 1;
  }

  auto cities = readCitiesFromCSV(argv[1]);
  auto db = createDB(cities);

  /*printf("**DB created successfully!\n"
    "Hash 1 collisions: %d\n"
    "Hash 2 collisions: %d\n"
    "KDTree nodes: %d (leafs: %d)\n\n",
    db->hash1.collisionCount,
    db->hash2.collisionCount,
    db->tree->nodeCount, db->tree->leafCount);
  */
  //testCities(cities);
  talktome(db);
  deleteDB(db);
  return 0;
}