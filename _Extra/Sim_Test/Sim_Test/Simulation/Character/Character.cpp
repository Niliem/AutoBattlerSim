#include "Character.h"
#include "Team.h"
#include <iostream>
#include <sstream>

#include "../Grid/Grid.h"

Character::Character(std::string name, Point startPos, int seed)
    : Entity(name)
      , name(name)
      , gen(seed)
      , position(startPos)
{
}

void Character::update(float deltaTime)
{
    if (auto targetPtr = target.lock())
    {
        // attack target
        if (currentAttackCooldown > 0.0f)
        {
            currentAttackCooldown -= deltaTime;
        }
        if (position.distance(targetPtr->getPosition()) <= attackRange)
        {
            if (currentAttackCooldown <= 0.0f)
            {
                float damage = getDamage();
                OnAttack(damage);
                targetPtr->takeDamage(damage);
                currentAttackCooldown = attackCooldown;
            }
        }        
        // move to target
        else if (auto gridPtr = grid.lock())
        {
            auto path = gridPtr->findPath(position, targetPtr->getPosition());
            if (path.size() > 0)
            {  
                moveProgress += deltaTime * moveSpeed;
                int pathIndex = 0;
                while (moveProgress >= 1.0f && pathIndex < path.size())
                {
                    position = path[pathIndex++];
                    OnMoveTo(position);
                    moveProgress -= 1.0f;
                    if (pathIndex >= path.size())
                    {
                        moveProgress = 0.0f;
                        path = gridPtr->findPath(position, targetPtr->getPosition());
                        break;
                    }
                }                
            }
        }        
    }
}

const std::string& Character::getName() const
{
    return name;
}

std::string Character::getNameCopy() const
{
    return name;
}

int Character::getHealth() const
{
    return health;
}

float Character::getHealthPercent() const
{
    return static_cast<float>(health) / maxHealth;
}

const Point& Character::getPosition() const
{
    return position;
}

void Character::setTeam(std::shared_ptr<Team> newTeam)
{
    if (auto teamPtr = team.lock())
    {
        teamPtr->removeMember(shared_from_this());
    }
    team = newTeam;
    newTeam->addMember(shared_from_this());
}

std::shared_ptr<Team> Character::getTeam() const
{
    return team.lock();
}

int Character::getDamage()
{
    std::uniform_int_distribution<int> distribution(minDamage, maxDamage);
    return distribution(gen);
}

void Character::takeDamage(int damage)
{
    health = std::ranges::max(0, health - damage);
    //std::cout << name << " took " << damage << " damage! HP: " << health << std::endl;
}

bool Character::isAlive() const
{
    return health > 0;
}

void Character::setTarget(std::weak_ptr<Character> newTarget)
{
    target = newTarget;
}

void Character::printStatus() const
{
    std::cout << name << " [HP: " << health << "] at (" << position.x << ", " << position.y << ")" << std::endl;
}

std::string Character::getStatus() const
{
    std::ostringstream oss;
    oss << name << " [HP: " << health << "] at (" << position.x << ", " << position.y << ")" << std::endl;
    return oss.str();
}

float Character::getMoveProgress() const
{
    return moveProgress;
}