#include "MoveAction.h"

#include "CombatSim/Simulation/Grid/Grid.h"
#include "Components/ProgressActionComponent.h"

MoveAction::MoveAction(float speed, Grid& grid, Point& position, std::function<Point()> target, std::function<float()> acceptanceRadius, int seed)
    : m_grid(grid)
    , m_position(position)
    , m_getTarget(target)
    , m_getAcceptanceRadius(acceptanceRadius)
    , m_gen(seed)
{
    addComponent(std::make_unique<ProgressActionComponent>(speed));
}

void MoveAction::onExecute()
{
    std::vector<Point> path = m_grid.findPath(m_position, m_getTarget(), m_getAcceptanceRadius());

    if (path.size() <= 1)
        return;

    m_position = path[1];
    OnExecute();
}

void MoveAction::onUpdate(float deltaTime)
{
    if (const auto& progressComponent = getComponent<ProgressActionComponent>())
    {
        if (progressComponent->isReady())
        {
            execute();
        }
    }
}