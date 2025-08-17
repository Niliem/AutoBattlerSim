#pragma once

#include "CombatSim/Simulation/Grid/Point.h"
#include "Entity.h"
#include <random>
#include <functional>

#include "CombatSim/Simulation/Components/HealthComp.h"

class Action;
class Grid;
class Team;

class Character : public Entity, public std::enable_shared_from_this<Character>
{
public:
    using Ptr = std::shared_ptr<Character>;

    Character(std::string name, Point startPos, int seed);

    bool operator==(const Character& other) const
    {
        return m_name == other.getName();
    }

    Action* addAction(std::unique_ptr<Action> action);

    void update(float deltaTime);

    const std::string& getName() const;    
    const Point& getPosition() const;

    void setTeam(std::shared_ptr<Team> newTeam);
    std::shared_ptr<Team> getTeam() const;

    bool isAlive() const;
    void takeDamage(int damage);

    bool hasValidTarget() const;
    std::weak_ptr<Character> getTarget() const;
    void setTarget(std::weak_ptr<Character> newTarget);

    void printStatus() const;
    std::string getStatus() const;
    
    void findClosestTarget(std::vector<std::shared_ptr<Team>>& teams, const Grid& grid);
    
    std::function<void(Point, int)> OnAttack;
    std::function<void(Point, Point, float)> OnMoveStarted;
    std::function<void(Point)> OnMoveFinished;
    std::function<void(float)> OnHealthChanged;
    std::function<void()> OnDead;

private:
    std::string m_name;
    std::mt19937 m_gen;

    Point m_position;

    std::weak_ptr<Team> m_team;

    int m_minInitialHealth = 2;
    int m_maxInitialHealth = 5;
    std::unique_ptr<HealthComp> m_healthComp;

    std::vector<std::unique_ptr<Action>> m_actions;

    std::weak_ptr<Character> m_target;
};