
#include <chrono>
#include <iostream>

#include "Simulation/Character/Character.h"
#include "Simulation/Character/Team.h"
#include "Simulation/Grid/Grid.h"



int main()
{
    int seed = 1234;
    
    auto grid = std::make_shared<Grid>(20, 20);

    auto team1 = std::make_shared<Team>("Red");
    auto team2 = std::make_shared<Team>("Blue");

    Point start1{1, 1};
    auto char1 = std::make_shared<Character>("Character 1", start1, seed);
    char1->setTeam(team1);
    char1->grid = grid;

    Point start2{19, 19};
    auto char2 = std::make_shared<Character>("Character 2", start2, seed + 1);
    char2->setTeam(team2);
    char2->grid = grid;

    team1->addMember(char1);
    team2->addMember(char2);

    grid->printGridWithPath({1, 1}, {19, 19});

    char1->printStatus();
    char2->printStatus();   

    auto lastTime = std::chrono::steady_clock::now();
    //float accumulatedTime = 0.0f;
    //float tickRate = 100.0f;
    //const float fixedDeltaTime = tickRate / 1000.0f;
    
    bool bSimRunning = true;
    while (bSimRunning)
    {
        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count();
        // accumulatedTime += deltaTime;
        // 
        // if (accumulatedTime < fixedDeltaTime)
        //     continue;
        // 
        // accumulatedTime -= fixedDeltaTime;
        
        char1->setTarget(team2->getAliveMembers()[0]);
        char2->setTarget(team1->getAliveMembers()[0]);
            
        char1->update(deltaTime);
        char2->update(deltaTime);

        char1->OnAttack = [&](float damage)
        {
            std::cout << char1->getName() << " is attacking with " << damage << std::endl;
        };
        char1->OnMoveTo = [&](Point newPosition)
        {
            std::cout << char1->getStatus() << std::endl;
        };

        char2->OnAttack = [&](float damage)
        {
            std::cout << char2->getName() << " is attacking with " << damage << std::endl;
        };
        char2->OnMoveTo = [&](Point newPosition)
        {
            std::cout << char2->getStatus() << std::endl;
        };

        if (team1->isDefeated() || team2->isDefeated())
        {
            bSimRunning = false;
            break;
        }
    }
    return 0;
}