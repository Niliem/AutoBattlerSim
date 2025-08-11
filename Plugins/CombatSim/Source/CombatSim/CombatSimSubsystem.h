// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatSimStructs.h"
#include "CombatSimTask.h"
#include "CombatSimSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSIM_API UCombatSimSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    //~ Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    //~ End USubsystem

    UFUNCTION(BlueprintCallable)
    void StartSimulation(int32 Seed = -1, int32 NumCharacters = 4);

    UFUNCTION(BlueprintCallable)
    void ClearSimulation();

    void SpawnSimEntity(FSimEntitySpawnInfo Info);
    void AttackSimEntity(FSimEntityAttackInfo Info);
    void UpdateHealthSimEntity(FSimEntityHealthChangedInfo Info);
    void DeadSimEntity(FSimEntityDeadInfo Info);
    void StartMoveSimEntity(FSimEntityStartMovementInfo Info);
    void FinishMoveSimEntity(FSimEntityFinishMovementInfo Info);

private:
    UPROPERTY()
    TMap<FString, TObjectPtr<AActor>> SimEntities;

    FAsyncTask<FCombatSimTask>* CombatSimTask;
};