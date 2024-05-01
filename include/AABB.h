#ifndef __Box2_H
#define __Box2_H

#include <cfloat>

struct Point2
{
    float x;
    float y;

}; // Point2

struct Box2
{
    Point2 p1; // left bottom corner
    Point2 p2; // right upper corner

}; // Box2

inline Box2
createEmptyBox2()
{
    Box2 box;

    box.p1.x = box.p1.y = +FLT_MAX;
    box.p2.x = box.p2.y = -FLT_MAX;
}

inline void
expand(Box2& box, const Point2 p)
{
    if (p.x < box.p1.x)
        box.p1.x = p.x;
    if (p.y < box.p1.y)
        box.p1.y = p.y;
    if (p.x > box.p2.x)
        box.p2.x = p.x;
    if (p.y > box.p2.y)
        box.p2.y = p.y;
}

inline auto
contains(const Box2& box, const Point2& p)
{
    return p.x >= box.p1.x &&
        p.y >= box.p1.y &&
        p.x <= box.p2.x &&
        p.y <= box.p2.y;
}


#endif // __Box2_H