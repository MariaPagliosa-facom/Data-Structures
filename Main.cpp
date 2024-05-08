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
    puts("Integer expected! Try again");
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
      auto knn = findKNN(db->tree, p, k + 1);

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

void
talktome(const DB* db)
{
  for (;;)
  {
    printf("MENU\n"
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
        if (auto ibge_code = readIbgeCode(); ibge_code != -1)
          printKNN(db, ibge_code, true);
        break;
      case '3':
        if (auto city_name = readCityName())
          printKNN(db, city_name);
        break;
      case '4':
        puts("Bye!");
        return;
      default:
        puts("Unknown option! Try again\n");
        break;
    }
  }
}

inline void
sortTest(Cities* cities)
{
  auto cptrs = createCityPtrs(cities);

  sortCities(cptrs, 0, 10, 0);
  sortCities(cptrs, 10, 5, 1);
  for (int i = 0; i < 15; ++i)
  {
    printf("**City %d\n", i + 1);
    printCity(*(cptrs.data[i]));
  }
  deleteCityPtrs(cptrs);
}

inline void
boxTest()
{
  Box2 box;

  box.min = Point2{2, 4};
  box.max = Point2{7, 8};
  printf("%f\n", distance(box, Point2{1, 6}));
  printf("%f\n", distance(box, Point2{8, 6}));
  printf("%f\n", distance(box, Point2{5, 9}));
  printf("%f\n", distance(box, Point2{5, 1}));
  printf("%f\n", distance(box, Point2{2, 5}));
  printf("%f\n", distance(box, Point2{5, 4}));
  printf("%f\n", distance(box, Point2{3, 5}));
}

int
main(int argc, char** argv)
{
  //boxTest();
  if (argc == 1)
  {
    puts("Usage:\nTest <city_file>");
    return 1;
  }

  auto cities = readCitiesFromCSV(argv[1]);
  auto db = createDB(cities);

  printf("**DB created successfully!\n"
    "Hash 1 collisions: %d\n"
    "Hash 2 collisions: %d\n"
    "KDTree nodes: %d (leafs: %d)\n\n",
    db->hash1.collisionCount,
    db->hash2.collisionCount,
    db->tree->nodeCount, db->tree->leafCount);

  //sortTest(cities);
  talktome(db);
  //printCities(cities, 10);
  deleteDB(db);
  return 0;
}