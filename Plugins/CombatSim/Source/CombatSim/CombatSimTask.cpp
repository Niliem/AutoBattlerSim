// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSimTask.h"

#include <format>

#include "Simulation/Entities/Character.h"
#include "Simulation/Entities/Obstacle.h"
#include "Simulation/Other/Team.h"
#include "Simulation/Grid/Grid.h"


void FCombatSimTask::DoWork()
{
    auto grid = std::make_shared<Grid>(100, 100, SimSeed);

    std::vector<std::shared_ptr<Obstacle>> obstacles;
    {
        const int leftBorder = 0;
        const int rightBorder = grid->getWidth() - 1;

        const int topBorder = 0;
        const int bottomBorder = grid->getHeight() - 1;

        for (int y = topBorder; y <= bottomBorder; ++y)
        {
            grid->setObstacle(leftBorder, y);
            SpawnObstacle(leftBorder, y, obstacles);

            grid->setObstacle(rightBorder, y);
            SpawnObstacle(rightBorder, y, obstacles);
        }

        for (int x = leftBorder + 1; x < rightBorder; ++x)
        {
            grid->setObstacle(x, topBorder);
            SpawnObstacle(x, topBorder, obstacles);

            grid->setObstacle(x, bottomBorder);
            SpawnObstacle(x, bottomBorder, obstacles);
        }

        for (int i = 0; i < 10; ++i)
        {
            Point position = grid->getRandomWalkablePosition();
            grid->setObstacle(position.x, position.y);
            SpawnObstacle(position.x, position.y, obstacles);
        }
    }

    std::vector<std::shared_ptr<Team>> teams;
    auto team1 = std::make_shared<Team>("Red");
    teams.push_back(team1);
    auto team2 = std::make_shared<Team>("Blue");
    teams.push_back(team2);

    std::vector<std::shared_ptr<Character>> characters;
    {
        std::mt19937 gen(SimSeed);
        std::uniform_int_distribution<int> distribution(0, teams.size() - 1);
        for (int i = 0; i < NumCharacters; ++i)
        {
            auto team = teams[distribution(gen)];
            SpawnCharacter(SimSeed, grid, team, characters);
        }
    }

    for (auto character : characters)
    {
        if (!character->hasValidTarget())
            character->findClosestTarget(teams);
        
        character->OnAttack = [&, character](Point targetPosition, int damage)
        {
            UE_LOG(LogTemp, Warning, TEXT("%hs is dealing %i damage"), character->getName().c_str(), damage);
            broadcastCharacterAttack(*character, targetPosition);
        };
        character->OnHealthChanged = [&, character](float healthPercent)
        {
            UE_LOG(LogTemp, Warning, TEXT("%hs was injured: %f"), character->getName().c_str(), healthPercent);
            broadcastCharacterHealthChanged(*character, healthPercent);
        };
        character->OnDead = [&, character]()
        {
            UE_LOG(LogTemp, Warning, TEXT("%hs is dead"), character->getName().c_str());
            broadcastCharacterDead(*character);
        };
        character->OnMoveStarted = [&, character](Point currentPosition, Point newPosition, float moveDuration)
        {
            broadcastCharacterStartMovement(*character, newPosition, moveDuration);
        };
        character->OnMoveFinished = [&, character](Point newPosition)
        {
            UE_LOG(LogTemp, Warning, TEXT("%hs moved to [%d, %d]"), character->getName().c_str(), newPosition.x, newPosition.y);
            character->findClosestTarget(teams);
            broadcastCharacterFinishMovement(*character);
        };
    }

    float deltaTime = 1.0f / 120.0f;

    bSimRunning = true;
    while (bSimRunning)
    {
        FPlatformProcess::Sleep(deltaTime);

        if (!bSimRunning)
            return;

        for (auto character : characters)
        {
            if (!character->hasValidTarget())
                character->findClosestTarget(teams);
            
            character->update(deltaTime);
        }

        int defeatedCount = 0;
        for (const auto& team : teams)
        {
            if (team->isDefeated())
                defeatedCount++;
        }
        if (defeatedCount >= teams.size() - 1)
        {
            bSimRunning = false;
            break;
        }
    }
}

void FCombatSimTask::Cancel()
{
    bSimRunning = false;
}

bool FCombatSimTask::CanAbandon()
{
    return true;
}

void FCombatSimTask::Abandon()
{
    bSimRunning = false;
}

void FCombatSimTask::broadcastEntitySpawned(const Entity& entity, const Point& position, const FString type) const
{
    FSimEntitySpawnInfo SimEntitySpawnInfo;
    SimEntitySpawnInfo.Id = FString(entity.getId().c_str());

    SimEntitySpawnInfo.Type = type;
    SimEntitySpawnInfo.Position = FVector::ZeroVector;
    SimEntitySpawnInfo.Position.X = position.x * GridSize;
    SimEntitySpawnInfo.Position.Y = position.y * GridSize;

    OnSimEntitySpawned.Execute(SimEntitySpawnInfo);
}

void FCombatSimTask::broadcastCharacterAttack(const Character& character, const Point& targetPosition) const
{
    FSimEntityAttackInfo SimEntityAttackInfo;
    SimEntityAttackInfo.Id = FString(std::string(character.getId()).c_str());

    SimEntityAttackInfo.TargetPosition = FVector::ZeroVector;
    SimEntityAttackInfo.TargetPosition.X = targetPosition.x * GridSize;
    SimEntityAttackInfo.TargetPosition.Y = targetPosition.y * GridSize;

    OnSimEntityAttack.Execute(SimEntityAttackInfo);
}

void FCombatSimTask::broadcastCharacterHealthChanged(const Character& character, float healthPercent) const
{
    FSimEntityHealthChangedInfo SimEntityHealthChangedInfo;
    SimEntityHealthChangedInfo.Id = FString(character.getId().c_str());

    SimEntityHealthChangedInfo.HealthPercent = healthPercent;

    OnSimEntityHealthChanged.Execute(SimEntityHealthChangedInfo);
}

void FCombatSimTask::broadcastCharacterDead(const Character& character) const
{
    FSimEntityDeadInfo SimEntityDeadInfo;
    SimEntityDeadInfo.Id = FString(character.getId().c_str());

    OnSimEntityDead.Execute(SimEntityDeadInfo);
}

void FCombatSimTask::broadcastCharacterStartMovement(const Character& character, const Point& targetPosition, float duration) const
{
    FSimEntityStartMovementInfo SimEntityStartMovementInfo;
    SimEntityStartMovementInfo.Id = FString(character.getId().c_str());

    SimEntityStartMovementInfo.CurrentPosition = FVector::ZeroVector;
    SimEntityStartMovementInfo.CurrentPosition.X = character.getPosition().x * GridSize;
    SimEntityStartMovementInfo.CurrentPosition.Y = character.getPosition().y * GridSize;
    SimEntityStartMovementInfo.NewPosition = FVector::ZeroVector;
    SimEntityStartMovementInfo.NewPosition.X = targetPosition.x * GridSize;
    SimEntityStartMovementInfo.NewPosition.Y = targetPosition.y * GridSize;
    SimEntityStartMovementInfo.MovementDuration = duration;

    OnSimEntityStartMovement.Execute(SimEntityStartMovementInfo);
}

void FCombatSimTask::broadcastCharacterFinishMovement(const Character& character) const
{
    FSimEntityFinishMovementInfo SimEntityFinishMovementInfo;
    SimEntityFinishMovementInfo.Id = FString(character.getId().c_str());

    SimEntityFinishMovementInfo.Position = FVector::ZeroVector;
    SimEntityFinishMovementInfo.Position.X = character.getPosition().x * GridSize;
    SimEntityFinishMovementInfo.Position.Y = character.getPosition().y * GridSize;

    OnSimEntityFinishMovement.Execute(SimEntityFinishMovementInfo);
}

void FCombatSimTask::SpawnObstacle(int x, int y, std::vector<std::shared_ptr<Obstacle>>& obstacles)
{
    std::string obstacleName = std::format("Obstacle_{},{}", x, y);
    auto obstacle = std::make_shared<Obstacle>(obstacleName, Point{x, y});
    obstacles.push_back(obstacle);
    broadcastEntitySpawned(*obstacle, obstacle->getPosition(), "Obstacle");
}

void FCombatSimTask::SpawnCharacter(int seed, std::shared_ptr<Grid> grid, std::shared_ptr<Team> team, std::vector<std::shared_ptr<Character>>& characters)
{
    Point position = grid->getRandomWalkablePosition();
    std::string name = std::format("Character_{},{}", position.x, position.y);
    auto character = std::make_shared<Character>(name, position, seed);
    character->setTeam(team);
    character->grid = grid;
    characters.push_back(character);
    broadcastEntitySpawned(*character, character->getPosition(), character->getTeam()->getName().c_str());
}