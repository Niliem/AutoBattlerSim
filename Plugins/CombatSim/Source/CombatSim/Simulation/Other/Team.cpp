#include "Team.h"
#include "../Entities/Character.h"
#include <algorithm>

Team::Team(std::string name)
    : m_name(name)
{
}

void Team::addMember(std::shared_ptr<Character> character)
{
    if (!character)
        return;

    if (std::find(m_members.begin(), m_members.end(), character) == m_members.end())
        m_members.push_back(character);
}

void Team::removeMember(std::shared_ptr<Character> character)
{
    if (!character)
        return;

    m_members.erase(std::remove(m_members.begin(), m_members.end(), character), m_members.end());
}

std::vector<std::shared_ptr<Character>> Team::getAliveMembers() const
{
    std::vector<std::shared_ptr<Character>> alive;
    for (const auto& member : m_members)
    {
        if (member->isAlive())
            alive.push_back(member);
    }
    return alive;
}

bool Team::isDefeated() const
{
    return getAliveMembers().empty();
}

const std::string& Team::getName() const
{
    return m_name;
}