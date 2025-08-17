#pragma once

enum class ActionComponentType
{
    None,
    Cooldown,
    Progress
};

class ActionComponent
{
public:
    virtual ~ActionComponent() = default;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onExecute() = 0;
    virtual bool isReady() const { return true; }
    ActionComponentType componentType;
};