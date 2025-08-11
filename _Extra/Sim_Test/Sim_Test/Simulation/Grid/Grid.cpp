#include "Grid.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <iostream>

Grid::Grid(int w, int h)
    : width(w)
    , height(h)
{
    grid.resize(height, std::vector<bool>(width, true));
}

void Grid::setObstacle(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        grid[y][x] = false;
    }
}

bool Grid::isWalkable(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height && grid[y][x];
}

std::vector<Point> Grid::getNeighbors(const Point& p) const
{
    std::vector<Point> neighbors;
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (const auto& [dx, dy] : directions)
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

std::vector<Point> Grid::findPath(const Point& start, const Point& end) const
{
    struct NodeInfo
    {
        float g = std::numeric_limits<float>::max();
        float f = std::numeric_limits<float>::max();
        Point prev = {-1, -1};
        bool visited = false;
    };

    auto cmp = [](const std::pair<Point, float>& a, const std::pair<Point, float>& b)
    {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<Point, float>, std::vector<std::pair<Point, float>>, decltype(cmp)> openSet(cmp);

    std::unordered_map<Point, NodeInfo> nodes;
    nodes[start].g = 0;
    nodes[start].f = start.distance(end);
    openSet.emplace(start, nodes[start].f);

    while (!openSet.empty())
    {
        Point current = openSet.top().first;
        openSet.pop();

        if (current == end)
        {
            std::vector<Point> path;
            Point p = end;
            while (p != start)
            {
                path.push_back(p);
                p = nodes[p].prev;
            }
            //path.push_back(start);
            std::ranges::reverse(path);
            return path;
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
                nodes[neighbor].f = tentative_g + neighbor.distance(end);

                if (!nodes[neighbor].visited)
                {
                    openSet.emplace(neighbor, nodes[neighbor].f);
                }
            }
        }
    }

    return {};
}

void Grid::printGridWithPath(const Point& start, const Point& end) const
{
    auto path = findPath(start, end);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Point p{x, y};
            if (std::ranges::find(path, p) != path.end())
                std::cout << "*";
            else
                std::cout << (grid[y][x] ? "." : "#");
        }
        std::cout << std::endl;
    }
}

std::string Grid::makeGridWithPath(const Point& start, const Point& end)
{
    std::string output;

    auto path = findPath(start, end);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Point p{x, y};
            if (std::ranges::find(path, p) != path.end())
                 output += "*";
            else
                output += (grid[y][x] ? "." : "#");
        }
        output += "\n";
    }

    return output;
}