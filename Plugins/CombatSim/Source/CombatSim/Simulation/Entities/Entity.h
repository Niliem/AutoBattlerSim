#pragma once

#include <string>

class Entity
{
public:
    Entity(std::string id);
    virtual ~Entity() = default;

    bool operator==(const Entity& other) const
    {
        return m_id == other.m_id;
    }

    // TODO: use GUID
    std::string m_id;

    const std::string& getId() const;
};