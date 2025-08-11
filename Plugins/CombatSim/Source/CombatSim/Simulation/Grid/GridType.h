#pragma once

#include <vector>

#include "Point.h"

class GridType
{
public:
    virtual ~GridType() = default;

    virtual std::vector<Point> getDirections() const = 0;
    virtual float getDistance(const Point& a, const Point& b) const = 0;
};

class SquareGridType : public GridType
{
public:
    virtual std::vector<Point> getDirections() const override
    {
        return {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    }

    virtual float getDistance(const Point& a, const Point& b) const override
    {
        return hypotf(a.x - b.x, a.y - b.y);
    }
};

class HexGridType : public GridType
{
public:
    virtual std::vector<Point> getDirections() const override
    {
        return {{1, 0}, {1, -1}, {0, -1}, {-1, 0}, {-1, 1}, {0, 1}};
    }

    virtual float getDistance(const Point& a, const Point& b) const override
    {
        int dx = b.x - a.x;
        int dy = b.y - a.y;
        return (abs(dx) + abs(dx + dy) + abs(dy)) / 2;
    }
};