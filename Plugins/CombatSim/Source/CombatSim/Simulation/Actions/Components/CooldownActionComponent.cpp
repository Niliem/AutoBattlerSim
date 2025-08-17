#include "CooldownActionComponent.h"

CooldownActionComponent::CooldownActionComponent(float cooldown)
    : m_cooldown(cooldown)
{
    componentType = ActionComponentType::Cooldown;
}

bool CooldownActionComponent::isReady() const
{
    return m_currentCooldown <= 0.0f;
}

void CooldownActionComponent::onExecute()
{
    m_currentCooldown = m_cooldown;
}

void CooldownActionComponent::onUpdate(float deltaTime)
{
    if (isReady())
        return;
        
    m_currentCooldown -= deltaTime;
}

float CooldownActionComponent::getCooldown() const
{
    return m_cooldown;
}

float CooldownActionComponent::getRemainingCooldown() const
{
    return m_currentCooldown;
}