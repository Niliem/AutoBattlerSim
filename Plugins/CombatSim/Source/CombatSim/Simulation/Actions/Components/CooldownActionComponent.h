#pragma once

#include "ActionComponent.h"

class CooldownActionComponent : public ActionComponent
{
public:
    CooldownActionComponent(float cooldown);

    //~ Begin ActionComponent Interface
    virtual bool isReady() const override;
    virtual void onExecute() override;
    virtual void onUpdate(float deltaTime) override;
    //~ End ActionComponent Interface

    float getCooldown() const;
    float getRemainingCooldown() const;

private:
    float m_cooldown = 0.0f;
    float m_currentCooldown = 0.0f;
};