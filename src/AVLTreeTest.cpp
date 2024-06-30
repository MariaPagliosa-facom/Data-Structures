#include "RangeTree.h"
#include <cstdio>

void
printCityInfo(const City& city, void* = nullptr)
{
  printf("%s %d\n", city.city_name, city.ddd);
}

extern int cmpDataByName(const City* city1, const City* city2);
extern int cmpByName(const City* city, const void* key);
extern int cmpDataByDDD(const City* city1, const City* city2);
extern int cmpByDDD(const City* city, const void* key);

void
AVLTest(Cities* cities)
{
  if (false)
  {
    auto tree = createAVLTree(cities, cmpDataByName, cmpByName);

    puts("\n**Tree by name");
    //iterate(tree, printCityInfo, 10);
    puts("Find test");
    if (auto city = find(tree, "Campo Grande"))
      printCityInfo(*city);
    else
      puts("City not found");
    findAll(tree, "Campo Grande", printCityInfo);
    puts("\nRange query test");
    rangeQuery(tree, {"Bonito", "Curitiba"}, printCityInfo);
    deleteAVLTree(tree);
  }
  if (true)
  {
    auto tree = createAVLTree(cities, cmpDataByDDD, cmpByDDD);

    puts("\n**Tree by DDD");
    //iterate(tree, printCityInfo, 10);
    puts("Find test");
    if (auto city = find(tree, (const void*)67))
      printCityInfo(*city);
    else
      puts("City not found");
    //findAll(tree, (const void*)67, printCityInfo);
    if (removeAll(tree, (const void*)98))
      puts("Cities with DDD 98 removed");
    puts("\nRange query test");
    if (auto res = rangeQuery(tree, {(const void*)86, (const void*)99}))
    {
      //iterate(res, printCityInfo);
      deleteAVLTree(res);
    }
    iterate(tree, printCityInfo);
    deleteAVLTree(tree);
  }
  puts("Press any key to exit...");
  (void)getchar();
}
