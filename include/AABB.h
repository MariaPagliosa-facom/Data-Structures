#ifndef __Box2_H
#define __Box2_H

#include <cassert>
#include <cfloat>
#include <cmath>

struct Point2
{
  float x[2];

}; // Point2

inline auto
distance(const Point2& p, const Point2& q)
{
  auto d0 = p.x[0] - q.x[0];
  auto d1 = p.x[1] - q.x[1];

  return sqrt(d0 * d0 + d1 * d1);
}

struct Box2
{
  Point2 min; // left bottom corner
  Point2 max; // right upper corner

}; // Box2

inline Box2
createEmptyBox2()
{
  Box2 box;

  box.min.x[0] = box.min.x[1] = +FLT_MAX;
  box.max.x[0] = box.max.x[1] = -FLT_MAX;
  return box;
}

inline Box2
createBox2(const Point2& min, const Point2& max)
{
  return Box2{min, max};
}

inline void
expand(Box2& box, const Point2 p)
{
  if (p.x[0] < box.min.x[0])
    box.min.x[0] = p.x[0];
  if (p.x[1] < box.min.x[1])
    box.min.x[1] = p.x[1];
  if (p.x[0] > box.max.x[0])
    box.max.x[0] = p.x[0];
  if (p.x[1] > box.max.x[1])
    box.max.x[1] = p.x[1];
}

inline auto
contains(const Box2& box, const Point2& p)
{
  return p.x[0] >= box.min.x[0] &&
    p.x[1] >= box.min.x[1] &&
    p.x[0] <= box.max.x[0] &&
    p.x[1] <= box.max.x[1];
}

inline void
split(const Box2& box, float x, int axis, Box2& b1, Box2& b2)
{
  assert(axis == 0 || axis == 1);
  assert(x >= box.min.x[axis] && x <= box.max.x[axis]);
  b1.min = b2.min = box.min;
  b1.max = b2.max = box.max;
  b1.max.x[axis] = b2.min.x[axis] = x;
}

inline auto
max(float a, float b, float c)
{             
  auto x = a > b ? a : b;
  return x > c ? x : c;
}

inline auto
distance(const Box2& box, const Point2& p)
{
  auto d0 = max(box.min.x[0] - p.x[0], 0, p.x[0] - box.max.x[0]);
  auto d1 = max(box.min.x[1] - p.x[1], 0, p.x[1] - box.max.x[1]);

  return sqrt(d0 * d0 + d1 * d1);
}

#endif // __Box2_H