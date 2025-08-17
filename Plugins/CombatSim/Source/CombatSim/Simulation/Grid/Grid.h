#pragma once

#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "GridType.h"
#include "Point.h"

class Obstacle;


class Grid
{
public:
    Grid(int w, int h, std::unique_ptr<GridType> gridType, int seed);
    virtual ~Grid() = default;

    struct NodeInfo
    {
        float g = std::numeric_limits<float>::max();
        float f = std::numeric_limits<float>::max();
        Point prev = {-1, -1};
        bool visited = false;
    };

    int getWidth() const;
    int getHeight() const;

    void setObstacle(int x, int y);

    bool isWalkable(int x, int y) const;
    Point getRandomWalkablePosition();
    
    std::vector<Point> getNeighbors(const Point& p) const;
    float getDistance(const Point& a, const Point& b) const;
    
    std::vector<Point> findPath(const Point& start, const Point& end, float acceptanceRadius = 0.0f) const;

    void printGridWithPath(const Point& start, const Point& end) const;
    std::string makeGridWithPath(const Point& start, const Point& end);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<bool>> m_grid;

    std::vector<Point> reconstructPath(const std::unordered_map<Point, NodeInfo>& nodes, Point end) const;
    
    std::vector<Point> getDirections() const;

    std::mt19937 m_gen;

    std::unique_ptr<GridType> m_gridType;


};