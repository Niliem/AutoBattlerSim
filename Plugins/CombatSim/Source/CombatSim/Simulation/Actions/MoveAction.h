#pragma once

#include "Action.h"
#include <functional>
#include <random>

class Grid;
struct Point;

class MoveAction : public Action
{
public:
    MoveAction(float speed, Grid& grid, Point& position, std::function<Point()> target, std::function<float()> acceptanceRadius, int seed);

    //~ Begin Action Interface
    virtual void onExecute() override;
    virtual void onUpdate(float deltaTime) override;
    //~ Begin Action Interface

private:
    Grid& m_grid;
    Point& m_position;
    std::function<Point()> m_getTarget;
    std::function<float()> m_getAcceptanceRadius;

    std::mt19937 m_gen;
};