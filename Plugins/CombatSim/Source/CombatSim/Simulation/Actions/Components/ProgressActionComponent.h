#pragma once

#include "ActionComponent.h"

class ProgressActionComponent : public ActionComponent
{
public:
    ProgressActionComponent(float speed = 1.0f, float progress = 1.0f);

    //~ Begin ActionComponent Interface
    virtual bool isReady() const override;
    virtual void onExecute() override;
    virtual void onUpdate(float deltaTime) override;
    //~ End ActionComponent Interface
    
    float getProgress() const;

private:
    float m_speed = 1.0f;
    float m_progress = 1.0f;
    float m_currentProgress = 0.0f;
};