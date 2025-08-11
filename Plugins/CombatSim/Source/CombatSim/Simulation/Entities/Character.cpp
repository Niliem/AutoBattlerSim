#include "Character.h"

#include <iostream>
#include <sstream>

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

void Character::update(float deltaTime)
{
    if (!isAlive())
        return;

    if (auto targetPtr = m_target.lock())
    {
        // TODO: Create Attack action
        // attack target
        if (m_currentAttackCooldown > 0.0f)
        {
            m_currentAttackCooldown -= deltaTime;
        }
        if (m_position.distance(targetPtr->getPosition()) <= m_attackRange)
        {
            if (m_currentAttackCooldown <= 0.0f)
            {
                float damage = getDamage();
                OnAttack(targetPtr->getPosition(), damage);
                targetPtr->takeDamage(damage);
                m_currentAttackCooldown = m_attackCooldown;
            }
        }

        // TODO: Create Move action
        // move to target
        else if (auto gridPtr = grid.lock())
        {
            auto path = gridPtr->findPath(m_position, targetPtr->getPosition());
            if (path.size() > 0)
            {
                if (m_moveProgress <= 0.0f)
                {
                    OnMoveStarted(m_position, path[0], 1.0f / m_moveSpeed);
                }
                m_moveProgress += deltaTime * m_moveSpeed;

                if (m_moveProgress >= 1.0f)
                {
                    int pathIndex = 0;
                    while (m_moveProgress >= 1.0f && pathIndex < path.size())
                    {
                        m_position = path[pathIndex++];
                        OnMoveFinished(m_position);
                        m_moveProgress -= 1.0f;

                        if (pathIndex < path.size())
                        {
                            Point newPosition = path[pathIndex];
                            OnMoveStarted(m_position, newPosition, 1.0f / m_moveSpeed);
                        }
                        else
                        {
                            m_moveProgress = 0.0f;
                            break;
                        }
                    }
                    
                }                
            }
        }
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

int Character::getDamage()
{
    std::uniform_int_distribution<int> distribution(m_minDamage, m_maxDamage);
    return distribution(m_gen);
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

void Character::findClosestTarget(std::vector<std::shared_ptr<Team>>& teams)
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

            targetsByDistance[member] = m_position.distance(member->getPosition());
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