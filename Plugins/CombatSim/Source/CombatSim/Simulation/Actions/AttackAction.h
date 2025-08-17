#pragma once

#include "Action.h"
#include <functional>
#include <random>

struct Point;

struct AttackConfig
{
    int minDamage;
    int maxDamage;
    float range;
    float cooldown;
    
    AttackConfig(int minDmg = 2, int maxDmg = 5, float rng = 2.0f, float cd = 2.0f)
        : minDamage(minDmg)
        , maxDamage(maxDmg)
        , range(rng)
        , cooldown(cd)
    {
    }
};

class AttackAction : public Action
{
public:
    AttackAction(const AttackConfig& attackConfig, Point& position, std::function<Point()> target, std::function<void(int)> applyDamage, int seed);

    //~ Begin Action Interface
    virtual void onExecute() override;
    virtual void onUpdate(float deltaTime) override;
    //~ Begin Action Interface

private:
    AttackConfig m_attackConfig;
    Point& m_position;
    std::function<Point()> m_getTarget;
    std::function<void(int)> m_applyDamage;
    
    std::mt19937 m_gen;
};