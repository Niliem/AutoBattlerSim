#pragma once

#include <random>
#include <string>
#include <vector>

#include "GridType.h"
#include "Point.h"

class Obstacle;


class Grid
{
public:
    Grid(int w, int h, std::unique_ptr<GridType> gridType, int seed);
    virtual ~Grid() = default;

    int getWidth() const;
    int getHeight() const;

    void setObstacle(int x, int y);

    bool isWalkable(int x, int y) const;
    Point getRandomWalkablePosition();
    
    std::vector<Point> getNeighbors(const Point& p) const;
    float getDistance(const Point& a, const Point& b) const;
    
    std::vector<Point> findPath(const Point& start, const Point& end) const;

    void printGridWithPath(const Point& start, const Point& end) const;
    std::string makeGridWithPath(const Point& start, const Point& end);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<bool>> m_grid;
    
    std::vector<Point> getDirections() const;

    std::mt19937 m_gen;

    std::unique_ptr<GridType> m_gridType;
};