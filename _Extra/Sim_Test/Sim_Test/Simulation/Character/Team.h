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
    std::string getNameCopy() const;

private:
    std::string name;
    std::vector<std::weak_ptr<Character>> members;
};