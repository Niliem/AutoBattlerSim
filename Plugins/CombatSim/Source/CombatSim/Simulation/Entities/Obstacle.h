#pragma once

#include "Entity.h"
#include "CombatSim/Simulation/Grid/Point.h"
#include <memory>

class Obstacle : public Entity, public std::enable_shared_from_this<Obstacle>
{
public:
    using Ptr = std::shared_ptr<Obstacle>;

    Obstacle(std::string name, Point spawnPosition);

    const Point& getPosition() const;

private:
    Point m_position;
};