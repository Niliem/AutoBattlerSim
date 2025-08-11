// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <vector>

#include "CoreMinimal.h"
#include "CombatSimStructs.h"


struct Point;
class Team;
class Grid;
class Entity;
class Character;
class Obstacle;


/**
 * 
 */
class FCombatSimTask
{

public:
    DECLARE_DELEGATE_OneParam(FOnSimEntitySpawnedDelegate, FSimEntitySpawnInfo);
    DECLARE_DELEGATE_OneParam(FOnSimEntityAttackDelegate, FSimEntityAttackInfo);
    DECLARE_DELEGATE_OneParam(FOnSimEntityHealthChangedDelegate, FSimEntityHealthChangedInfo);
    DECLARE_DELEGATE_OneParam(FOnSimEntityDeadDelegate, FSimEntityDeadInfo);
    DECLARE_DELEGATE_OneParam(FOnSimEntityStartMovementDelegate, FSimEntityStartMovementInfo);
    DECLARE_DELEGATE_OneParam(FOnSimEntityFinishMovementDelegate, FSimEntityFinishMovementInfo);


    FCombatSimTask(
        int32 InSimSeed, int32 InNumCharacters
        , FOnSimEntitySpawnedDelegate&& InOnSimEntitySpawned
        , FOnSimEntityAttackDelegate&& InOnSimEntityAttack
        , FOnSimEntityHealthChangedDelegate&& InOnOnSimEntityHealthChanged
        , FOnSimEntityDeadDelegate&& InOnSimEntityDead
        , FOnSimEntityStartMovementDelegate&& InOnSimEntityStartMovement
        , FOnSimEntityFinishMovementDelegate&& InOnSimEntityFinishMovement)
        : SimSeed(InSimSeed), NumCharacters(InNumCharacters)
          , OnSimEntitySpawned(MoveTemp(InOnSimEntitySpawned))
          , OnSimEntityAttack(MoveTemp(InOnSimEntityAttack))
          , OnSimEntityHealthChanged(MoveTemp(InOnOnSimEntityHealthChanged))
          , OnSimEntityDead(MoveTemp(InOnSimEntityDead))
          , OnSimEntityStartMovement(MoveTemp(InOnSimEntityStartMovement))
          , OnSimEntityFinishMovement(MoveTemp(InOnSimEntityFinishMovement))
    {
    };

    void DoWork();
    void Cancel();

    bool CanAbandon();
    void Abandon();

    FORCEINLINE TStatId GetStatId() const
    {
        RETURN_QUICK_DECLARE_CYCLE_STAT(FCombatSimTask, STATGROUP_ThreadPoolAsyncTasks);
    }

private:
    void broadcastEntitySpawned(const Entity& entity, const Point& position, const FString type) const;

    void broadcastCharacterAttack(const Character& character, const Point& targetPosition) const;
    void broadcastCharacterHealthChanged(const Character& character, float healthPercent) const;
    void broadcastCharacterDead(const Character& character) const;
    void broadcastCharacterStartMovement(const Character& character, const Point& targetPosition, float duration) const;
    void broadcastCharacterFinishMovement(const Character& character) const;

    void SpawnObstacle(int x, int y, std::vector<std::shared_ptr<Obstacle>>& obstacles);
    void SpawnCharacter(int seed, std::shared_ptr<Grid> grid, std::shared_ptr<Team> team, std::vector<std::shared_ptr<Character>>& characters);

    int32 SimSeed = -1;
    int NumCharacters = 2;
    bool bSimRunning = false;

    FOnSimEntitySpawnedDelegate OnSimEntitySpawned;
    FOnSimEntityAttackDelegate OnSimEntityAttack;
    FOnSimEntityHealthChangedDelegate OnSimEntityHealthChanged;
    FOnSimEntityDeadDelegate OnSimEntityDead;
    FOnSimEntityStartMovementDelegate OnSimEntityStartMovement;
    FOnSimEntityFinishMovementDelegate OnSimEntityFinishMovement;

    float GridSize = 100.0f;
};