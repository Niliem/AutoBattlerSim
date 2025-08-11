#pragma once

#include "string"

class Entity
{
public:
    Entity(std::string id);
    virtual ~Entity() = default;

    bool operator==(const Entity& other) const {
        return id == other.id;
    }
    
    std::string id;
    
    const std::string& getId() const;
    std::string getIdCopy() const;
};