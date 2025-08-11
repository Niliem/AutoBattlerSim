#include "Entity.h"

Entity::Entity(std::string id)
    : id{id}
{
}

const std::string& Entity::getId() const
{
    return id;
}

std::string Entity::getIdCopy() const
{
    return id;
}