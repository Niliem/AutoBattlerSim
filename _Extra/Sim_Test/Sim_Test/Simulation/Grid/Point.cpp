#include "Point.h"
#include <corecrt_math.h>

Point::Point(int x, int y)
    : x{x}
    , y{y}
{
}

float Point::distance(const Point& other) const
{
    return hypotf(x - other.x, y - other.y);
}