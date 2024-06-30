#ifndef __TESTS_H
#define __TESTS_H

#include "Cities.h"
#include <cstdio>

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

extern void AVLTest(Cities* cities);

inline void
testCities(Cities* cities)
{
  //printCities(cities, 10);
  //sortTest(cities);
  AVLTest(cities);
}

#endif // __TESTS_H
