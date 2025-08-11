#include "Team.h"
#include "Character.h"
#include <algorithm>

Team::Team(std::string name)
    : name(name)
{
}

void Team::addMember(std::shared_ptr<Character> character)
{
    //if (std::find(members.begin(), members.end(), character) == members.end())
        members.push_back(character);
}

void Team::removeMember(std::shared_ptr<Character> character)
{
    //members.erase(std::remove(members.begin(), members.end(), character), members.end());
}

std::vector<std::shared_ptr<Character>> Team::getAliveMembers() const
{
    std::vector<std::shared_ptr<Character>> alive;
    for (const auto& weakMember : members)
    {
        if (auto member = weakMember.lock())
        {
            if (member->isAlive())
                alive.push_back(member);
        }
    }
    return alive;
}

bool Team::isDefeated() const
{
    return getAliveMembers().empty();
}

const std::string& Team::getName() const
{
    return name;
}

std::string Team::getNameCopy() const
{
    return name;
}