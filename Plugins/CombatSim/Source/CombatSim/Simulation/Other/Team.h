#pragma once

#include <memory>
#include <string>
#include <vector>


class Character;

class Team : public std::enable_shared_from_this<Team>
{
public:
    using Ptr = std::shared_ptr<Team>;

    Team(std::string name);

    void addMember(std::shared_ptr<Character> character);
    void removeMember(std::shared_ptr<Character> character);

    std::vector<std::shared_ptr<Character>> getAliveMembers() const;

    bool isDefeated() const;

    const std::string& getName() const;

private:
    std::string m_name;
    std::vector<std::shared_ptr<Character>> m_members;
};