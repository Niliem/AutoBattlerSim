#include "HealthComp.h"

HealthComp::HealthComp(int maxHealth)
    : m_health(maxHealth)
    , m_maxHealth(maxHealth)
{
}

int HealthComp::getHealth() const
{
    return m_health;
}

float HealthComp::getHealthPercent() const
{
    return static_cast<float>(m_health) / m_maxHealth;
}

void HealthComp::decreaseHealth(int amount)
{
    m_health = std::ranges::max(0, m_health - amount);
    OnHealthChanged(getHealthPercent());
}