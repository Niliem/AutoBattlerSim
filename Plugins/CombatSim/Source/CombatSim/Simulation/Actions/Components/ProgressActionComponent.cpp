#include "ProgressActionComponent.h"

ProgressActionComponent::ProgressActionComponent(float speed, float progress)
    : m_speed(speed)
    , m_progress(progress)
{
    componentType = ActionComponentType::Progress;
}

bool ProgressActionComponent::isReady() const
{
    return m_currentProgress >= m_progress;
}

void ProgressActionComponent::onExecute()
{
    m_currentProgress -= m_progress;
}

void ProgressActionComponent::onUpdate(float deltaTime)
{
    if (isReady())
        return;
        
    m_currentProgress += deltaTime * m_speed;
}

float ProgressActionComponent::getProgress() const
{
    return m_currentProgress / m_progress;
}