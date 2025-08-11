#pragma once
#include <functional>

class HealthComp
{
public:
    HealthComp(int maxHealth = 100);
    
    int getHealth() const;
    float getHealthPercent() const;
    void decreaseHealth(int amount);

    std::function<void(float)> OnHealthChanged;

private:
    int m_health = 100;
    int m_maxHealth = 100;
};