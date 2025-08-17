#include "AttackAction.h"

#include "CombatSim/Simulation/Grid/Point.h"
#include "Components/CooldownActionComponent.h"

AttackAction::AttackAction(const AttackConfig& attackConfig, Point& position, std::function<Point()> target, std::function<void(int)> applyDamage, int seed)
    : m_attackConfig(attackConfig)
    , m_position(position)
    , m_getTarget(target)
    , m_applyDamage(applyDamage)
    , m_gen(seed)
{
    addComponent(std::make_unique<CooldownActionComponent>(attackConfig.cooldown));
}

void AttackAction::onExecute()
{
    std::uniform_int_distribution<int> distribution(m_attackConfig.minDamage, m_attackConfig.maxDamage);
    m_applyDamage(distribution(m_gen));
}

void AttackAction::onUpdate(float deltaTime)
{
    if (m_position.distance(m_getTarget()) <= m_attackConfig.range)
    {
        if (const auto& cooldownComponent = getComponent<CooldownActionComponent>())
        {
            if (cooldownComponent->isReady())
            {
                execute();
            }
        }
    }
}