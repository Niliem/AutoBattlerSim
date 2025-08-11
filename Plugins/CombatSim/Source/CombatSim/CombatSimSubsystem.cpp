// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSimSubsystem.h"

#include "CombatSimInterface.h"
#include "Settings/CombatSimDeveloperSettings.h"
#include "Simulation/Entities/Character.h"
#include "Simulation/Other/Team.h"

void UCombatSimSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UCombatSimSubsystem::Deinitialize()
{
    ClearSimulation();

    Super::Deinitialize();
}

void UCombatSimSubsystem::StartSimulation(int32 Seed, int32 NumCharacters)
{
    ClearSimulation();

    auto OnSimSpawnedLambda = [this](FSimEntitySpawnInfo Info)
    {
        AsyncTask(ENamedThreads::GameThread, [this, Info]()
        {
            SpawnSimEntity(Info);
        });
    };

    auto OnCharacterAttackLambda = [this](FSimEntityAttackInfo Info)
    {
        AsyncTask(ENamedThreads::GameThread, [this, Info]()
        {
            AttackSimEntity(Info);
        });
    };

    auto OnCharacterHealthChangedLambda = [this](FSimEntityHealthChangedInfo Info)
    {
        AsyncTask(ENamedThreads::GameThread, [this, Info]()
        {
            UpdateHealthSimEntity(Info);
        });
    };

    auto OnCharacterDeadLambda = [this](FSimEntityDeadInfo Info)
    {
        AsyncTask(ENamedThreads::GameThread, [this, Info]()
        {
            DeadSimEntity(Info);
        });
    };

    auto OnCharacterStartMovementLambda = [this](FSimEntityStartMovementInfo Info)
    {
        AsyncTask(ENamedThreads::GameThread, [this, Info]()
        {
            StartMoveSimEntity(Info);
        });
    };

    auto OnCharacterFinishMovementLambda = [this](FSimEntityFinishMovementInfo Info)
    {
        AsyncTask(ENamedThreads::GameThread, [this, Info]()
        {
            FinishMoveSimEntity(Info);
        });
    };

    FCombatSimTask::FOnSimEntitySpawnedDelegate OnSimSpawnedDelegate = FCombatSimTask::FOnSimEntitySpawnedDelegate::CreateLambda(OnSimSpawnedLambda);
    FCombatSimTask::FOnSimEntityAttackDelegate OnCharacterAttackDelegate = FCombatSimTask::FOnSimEntityAttackDelegate::CreateLambda(OnCharacterAttackLambda);
    FCombatSimTask::FOnSimEntityHealthChangedDelegate OnCharacterHealthChangedDelegate = FCombatSimTask::FOnSimEntityHealthChangedDelegate::CreateLambda(OnCharacterHealthChangedLambda);
    FCombatSimTask::FOnSimEntityDeadDelegate OnCharacterDeadDelegate = FCombatSimTask::FOnSimEntityDeadDelegate::CreateLambda(OnCharacterDeadLambda);
    FCombatSimTask::FOnSimEntityStartMovementDelegate OnCharacterStartMovementDelegate = FCombatSimTask::FOnSimEntityStartMovementDelegate::CreateLambda(OnCharacterStartMovementLambda);
    FCombatSimTask::FOnSimEntityFinishMovementDelegate OnCharacterFinishMovementDelegate = FCombatSimTask::FOnSimEntityFinishMovementDelegate::CreateLambda(OnCharacterFinishMovementLambda);

    CombatSimTask = new FAsyncTask<FCombatSimTask>(Seed, NumCharacters,
        MoveTemp(OnSimSpawnedDelegate),
        MoveTemp(OnCharacterAttackDelegate),
        MoveTemp(OnCharacterHealthChangedDelegate),
        MoveTemp(OnCharacterDeadDelegate),
        MoveTemp(OnCharacterStartMovementDelegate),
        MoveTemp(OnCharacterFinishMovementDelegate)
        );
    CombatSimTask->StartBackgroundTask();
}

void UCombatSimSubsystem::ClearSimulation()
{
    if (CombatSimTask)
    {
        CombatSimTask->TryAbandonTask();
        CombatSimTask->Cancel();
    }

    for (auto entity : SimEntities)
    {
        if (entity.Value.Get())
            entity.Value.Get()->Destroy();
    }
    SimEntities.Empty();
}

void UCombatSimSubsystem::SpawnSimEntity(FSimEntitySpawnInfo Info)
{
    if (auto SimEntity = SimEntities.Find(Info.Id))
    {
        SimEntities.Remove(Info.Id);
        if (SimEntity->Get())
            SimEntity->Get()->Destroy();
    }

    auto CombatSimDeveloperSettings = GetDefault<UCombatSimDeveloperSettings>();
    if (!CombatSimDeveloperSettings)
        return;

    if (!CombatSimDeveloperSettings->EntityClassesForType.Contains(Info.Type))
        return;

    TSubclassOf<AActor> EntityClass = CombatSimDeveloperSettings->EntityClassesForType[Info.Type];

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(Info.Position);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* EntityActor = GetWorld()->SpawnActor<AActor>(EntityClass, SpawnTransform, SpawnParams);

    SimEntities.Add(Info.Id, EntityActor);
}

void UCombatSimSubsystem::AttackSimEntity(FSimEntityAttackInfo Info)
{
    if (auto SimEntity = SimEntities.Find(Info.Id))
    {
        if (SimEntity->Get() && SimEntity->Get()->GetClass()->ImplementsInterface(UCombatSimInterface::StaticClass()))
        {
            ICombatSimInterface::Execute_Attack(SimEntity->Get(), Info.TargetPosition);
        }
    }
}

void UCombatSimSubsystem::UpdateHealthSimEntity(FSimEntityHealthChangedInfo Info)
{
    if (auto SimEntity = SimEntities.Find(Info.Id))
    {
        if (SimEntity->Get() && SimEntity->Get()->GetClass()->ImplementsInterface(UCombatSimInterface::StaticClass()))
        {
            ICombatSimInterface::Execute_UpdateHealth(SimEntity->Get(), Info.HealthPercent);
        }
    }
}

void UCombatSimSubsystem::DeadSimEntity(FSimEntityDeadInfo Info)
{
    if (auto SimEntity = SimEntities.Find(Info.Id))
    {
        if (SimEntity->Get() && SimEntity->Get()->GetClass()->ImplementsInterface(UCombatSimInterface::StaticClass()))
        {
            ICombatSimInterface::Execute_Dead(SimEntity->Get());
        }
    }
}

void UCombatSimSubsystem::StartMoveSimEntity(FSimEntityStartMovementInfo Info)
{
    if (auto SimEntity = SimEntities.Find(Info.Id))
    {
        if (SimEntity->Get() && SimEntity->Get()->GetClass()->ImplementsInterface(UCombatSimInterface::StaticClass()))
        {
            ICombatSimInterface::Execute_StartMovement(SimEntity->Get(), Info.CurrentPosition, Info.NewPosition, Info.MovementDuration);
        }
    }
}

void UCombatSimSubsystem::FinishMoveSimEntity(FSimEntityFinishMovementInfo Info)
{
    if (auto SimEntity = SimEntities.Find(Info.Id))
    {
        if (SimEntity->Get() && SimEntity->Get()->GetClass()->ImplementsInterface(UCombatSimInterface::StaticClass()))
        {
            ICombatSimInterface::Execute_FinishMovement(SimEntity->Get(), Info.Position);
        }
    }
}