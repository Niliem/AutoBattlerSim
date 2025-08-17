#include "Grid.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <iostream>

#include "GridType.h"

Grid::Grid(int w, int h, std::unique_ptr<GridType> gridType, int seed)
    : m_width(w)
      , m_height(h)
      , m_gen(seed)
      , m_gridType(std::move(gridType))
{
    m_grid.resize(m_height, std::vector<bool>(m_width, true));
}

int Grid::getWidth() const
{
    return m_width;
}

int Grid::getHeight() const
{
    return m_height;
}

void Grid::setObstacle(int x, int y)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        m_grid[y][x] = false;
    }
}

bool Grid::isWalkable(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height && m_grid[y][x];
}

Point Grid::getRandomWalkablePosition()
{
    std::vector<Point> walkablePositions;

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            if (isWalkable(x, y))
                walkablePositions.emplace_back(x, y);
        }
    }

    if (walkablePositions.empty())
        return {-1, -1};

    std::uniform_int_distribution<int> distribution(0, walkablePositions.size() - 1);
    return walkablePositions[distribution(m_gen)];
}

std::vector<Point> Grid::getNeighbors(const Point& p) const
{
    std::vector<Point> neighbors;

    for (const auto& [dx, dy] : getDirections())
    {
        int nx = p.x + dx;
        int ny = p.y + dy;
        if (isWalkable(nx, ny))
        {
            neighbors.emplace_back(nx, ny);
        }
    }
    return neighbors;
}

float Grid::getDistance(const Point& a, const Point& b) const
{
    if (m_gridType)
        return m_gridType->getDistance(a, b);
    return hypotf(a.x - b.x, a.y - b.y);
}

std::vector<Point> Grid::findPath(const Point& start, const Point& end, float acceptanceRadius) const
{
    auto cmp = [](const std::pair<Point, float>& a, const std::pair<Point, float>& b)
    {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<Point, float>, std::vector<std::pair<Point, float>>, decltype(cmp)> openSet(cmp);

    std::unordered_map<Point, NodeInfo> nodes;
    nodes[start].g = 0;
    nodes[start].f = getDistance(start, end);
    openSet.emplace(start, nodes[start].f);

    Point closestPoint = start;
    float closestDistance = getDistance(start, end);

    while (!openSet.empty())
    {
        Point current = openSet.top().first;
        openSet.pop();

        if (current.distance(end) <= acceptanceRadius)
        {
            return reconstructPath(nodes, current);
        }
        
        float currentDistance = current.distance(end);
        if (currentDistance < closestDistance)
        {
            closestDistance = currentDistance;
            closestPoint = current;
        }

        if (nodes[current].visited)
            continue;
        nodes[current].visited = true;

        for (const auto& neighbor : getNeighbors(current))
        {
            float tentative_g = nodes[current].g + 1.0f;

            if (tentative_g < nodes[neighbor].g)
            {
                nodes[neighbor].prev = current;
                nodes[neighbor].g = tentative_g;
                nodes[neighbor].f = tentative_g + getDistance(neighbor, end);

                if (!nodes[neighbor].visited)
                {
                    openSet.emplace(neighbor, nodes[neighbor].f);
                }
            }
        }
    }
    
    if (acceptanceRadius > 0.0f && closestPoint != start)
    {
        return reconstructPath(nodes, closestPoint);
    }

    return {};
}

void Grid::printGridWithPath(const Point& start, const Point& end) const
{
    auto path = findPath(start, end);
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            Point p{x, y};
            if (std::ranges::find(path, p) != path.end())
                std::cout << "*";
            else
                std::cout << (m_grid[y][x] ? "." : "#");
        }
        std::cout << std::endl;
    }
}

std::string Grid::makeGridWithPath(const Point& start, const Point& end)
{
    std::string output;

    auto path = findPath(start, end);
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            Point p{x, y};
            if (std::ranges::find(path, p) != path.end())
                output += "*";
            else
                output += (m_grid[y][x] ? "." : "#");
        }
        output += "\n";
    }

    return output;
}

std::vector<Point> Grid::reconstructPath(const std::unordered_map<Point, NodeInfo>& nodes, Point end) const
{
    std::vector<Point> path;
    Point current = end;
    
    while (nodes.at(current).prev != Point(-1, -1)) {
        path.push_back(current);
        current = nodes.at(current).prev;
    }
    path.push_back(current);
    
    std::ranges::reverse(path);
    return path;
}

std::vector<Point> Grid::getDirections() const
{
    if (m_gridType)
        return m_gridType->getDirections();
    return {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
}