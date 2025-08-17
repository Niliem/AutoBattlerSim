#include "Action.h"

void Action::addComponent(std::unique_ptr<ActionComponent> component)
{
    m_components.emplace_back(std::move(component));
}

void Action::execute()
{
    for (const auto& component : m_components)
    {
        component->onExecute();
    }
    onExecute();
}

void Action::update(float deltaTime)
{
    for (const auto& component : m_components)
    {
        component->onUpdate(deltaTime);
    }
    onUpdate(deltaTime);
}