#pragma once

#include "Components/ActionComponent.h"
#include <functional>
#include <memory>
#include <vector>

class ProgressActionComponent;
class CooldownActionComponent;

class Action
{
public:
    Action() = default;    
    virtual ~Action() = default;

    void addComponent(std::unique_ptr<ActionComponent> component);

    template <typename T>
    T* getComponent()
    {
        static_assert(std::is_base_of_v<ActionComponent, T>, "T must be derived from ActionComponent");

        constexpr ActionComponentType targetType = getComponentType<T>();
        auto it = std::find_if(m_components.begin(), m_components.end(), [](const auto& component)
        {
            return component->componentType == targetType;
        });

        return it != m_components.end() ? static_cast<T*>(it->get()) : nullptr;
    }

    void execute();
    void update(float deltaTime);

    std::function<void()> OnExecute;

protected:
    virtual void onExecute() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    
protected:
    std::vector<std::unique_ptr<ActionComponent>> m_components;

private:
    // Error C4541 : 'typeid' used on polymorphic type 'ActionComponent' with /GR-; unpredictable behavior may result
    // Error C4541 : 'dynamic_cast' used on polymorphic type 'ActionComponent' with /GR-; unpredictable behavior may result
    template <typename T>
    static constexpr ActionComponentType getComponentType()
    {
        if constexpr (std::is_same_v<CooldownActionComponent, T>)
        {
            return ActionComponentType::Cooldown;
        }
        if constexpr (std::is_same_v<ProgressActionComponent, T>)
        {
            return ActionComponentType::Progress;
        }
        else
        {
            return ActionComponentType::None;
        }
    }
};