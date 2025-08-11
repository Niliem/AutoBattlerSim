#pragma once

#include <string>
#include <vector>
#include "Point.h"

class Grid
{
public:
    Grid(int w, int h);

    void setObstacle(int x, int y);
    
    bool isWalkable(int x, int y) const;

    std::vector<Point> getNeighbors(const Point& p) const;

    std::vector<Point> findPath(const Point& start, const Point& end) const;
    
    void printGridWithPath(const Point& start, const Point& end) const;
    std::string makeGridWithPath(const Point& start, const Point& end);

    int width, height;    
private:
    std::vector<std::vector<bool>> grid;
    
};