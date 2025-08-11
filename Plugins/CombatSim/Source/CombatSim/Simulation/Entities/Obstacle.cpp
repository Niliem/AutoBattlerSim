#include "Obstacle.h"

Obstacle::Obstacle(std::string name, Point spawnPosition)
    : Entity(name)
      , m_position(spawnPosition)
{
}

const Point& Obstacle::getPosition() const
{
    return m_position;
}