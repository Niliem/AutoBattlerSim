#include "Entity.h"

Entity::Entity(std::string id)
    : m_id(id)
{
}

const std::string& Entity::getId() const
{
    return m_id;
}