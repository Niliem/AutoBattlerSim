#include "Character.h"

#include <iostream>
#include <sstream>

#include "CombatSim/Simulation/Actions/Action.h"
#include "CombatSim/Simulation/Other/Team.h"
#include "CombatSim/Simulation/Grid/Grid.h"
#include "CombatSim/Simulation/Components/HealthComp.h"

Character::Character(std::string name, Point startPos, int seed)
    : Entity(name)
    , m_name(name)
    , m_gen(seed)
    , m_position(startPos)
{
    std::uniform_int_distribution<int> healthDist(m_minInitialHealth, m_maxInitialHealth);
    m_healthComp = std::make_unique<HealthComp>(healthDist(m_gen));
    m_healthComp->OnHealthChanged = [this](float healthPercent)
    {
        OnHealthChanged(healthPercent);
        if (healthPercent <= 0.0f)
            OnDead();
    };
}

Action* Character::addAction(std::unique_ptr<Action> action)
{
    Action* act = action.get();
    m_actions.emplace_back(std::move(action));
    return act;
}

void Character::update(float deltaTime)
{
    if (!isAlive())
        return;

    for (const auto& action : m_actions)
    {
        action->update(deltaTime);
    }
}

const std::string& Character::getName() const
{
    return m_name;
}

const Point& Character::getPosition() const
{
    return m_position;
}

void Character::setTeam(std::shared_ptr<Team> newTeam)
{
    if (auto teamPtr = m_team.lock())
    {
        teamPtr->removeMember(shared_from_this());
    }
    m_team = newTeam;
    newTeam->addMember(shared_from_this());
}

std::shared_ptr<Team> Character::getTeam() const
{
    return m_team.lock();
}

void Character::takeDamage(int damage)
{
    m_healthComp->decreaseHealth(damage);
}

bool Character::isAlive() const
{
    return m_healthComp->getHealth() > 0;
}

bool Character::hasValidTarget() const
{
    return (m_target.lock() && m_target.lock()->isAlive());
}

std::weak_ptr<Character> Character::getTarget() const
{
    return m_target;
}

void Character::setTarget(std::weak_ptr<Character> newTarget)
{
    m_target = newTarget;
}

void Character::printStatus() const
{
    std::cout << m_name << "[HP: " << m_healthComp->getHealth() << "] at (" << m_position.x << ", " << m_position.y << ")" << std::endl;
}

std::string Character::getStatus() const
{
    std::ostringstream oss;
    oss << m_name << "[HP: " << m_healthComp->getHealth() << "] at (" << m_position.x << ", " << m_position.y << ")" << std::endl;
    return oss.str();
}

void Character::findClosestTarget(std::vector<std::shared_ptr<Team>>& teams, const Grid& grid)
{
    std::unordered_map<std::shared_ptr<Character>, float> targetsByDistance;

    auto currentTeam = m_team.lock();
    for (const auto& ext_team : teams)
    {
        if (currentTeam == ext_team)
            continue;

        if (ext_team->isDefeated())
            continue;

        for (const auto& member : ext_team->getAliveMembers())
        {
            if (member.get() == this)
                continue;

            targetsByDistance[member] = grid.getDistance(m_position, member->getPosition());
        }
    }

    std::shared_ptr<Character> closest = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (const auto& [member, distance] : targetsByDistance)
    {
        if (distance < minDistance)
        {
            minDistance = distance;
            closest = member;
        }
    }

    setTarget(closest);
}