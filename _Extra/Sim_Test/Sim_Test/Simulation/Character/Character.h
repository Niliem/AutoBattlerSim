#pragma once

#include <functional>

#include "../Grid/Point.h"
#include "../Entity.h"
#include <random>


class Grid;
class Team;

class Character : public Entity, public std::enable_shared_from_this<Character>
{
public:
    using Ptr = std::shared_ptr<Character>;
    
    Character(std::string name, Point startPos, int seed);

    bool operator==(const Character& other) const {
        return name == other.name;
    }

    bool operator!=(const Character& other) const {
        return name != other.name;
    }
    
    void update(float deltaTime);

    const std::string& getName() const;
    std::string getNameCopy() const;

    int getHealth() const;
    float getHealthPercent() const;

    const Point& getPosition() const;

    void setTeam(std::shared_ptr<Team> newTeam);
    
    std::shared_ptr<Team> getTeam() const;

    int getDamage();

    void takeDamage(int damage);

    bool isAlive() const;

    void setTarget(std::weak_ptr<Character> newTarget);

    void printStatus() const;
    std::string getStatus() const;

    float getMoveProgress() const;

    // TODO:
    std::weak_ptr<Grid> grid;

    std::function<void(float)> OnAttack;
    std::function<void(Point)> OnMoveTo;

private:
    std::string name;
    std::mt19937 gen;

    Point position;

    std::weak_ptr<Team> team;

    int health = 100;
    int maxHealth = 100;

    float attackCooldown = 2.0f;
    float currentAttackCooldown = 0.0f;
    float attackRange = 1.0f;
    int minDamage = 3;
    int maxDamage = 5;

    float moveSpeed = 1.0f;
    float moveProgress = 0.0f;
    
    std::weak_ptr<Character> target;
};