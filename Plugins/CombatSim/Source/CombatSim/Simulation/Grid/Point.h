#pragma once

#include <tuple>

struct Point
{
    int x, y;

    Point(int x = 0, int y = 0);

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Point& other) const
    {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
    
    float distance(const Point& other) const;
};

namespace std
{
    template <>
    struct hash<Point>
    {
        size_t operator()(const Point& p) const
        {
            return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        }
    };
} // namespace std