// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatSimStructs.generated.h"

USTRUCT()
struct FSimEntitySpawnInfo
{
    GENERATED_BODY()
    
    FString Id;
    
    FString Type;
    FVector Position;
};

USTRUCT()
struct FSimEntityAttackInfo
{
    GENERATED_BODY()
    
    FString Id;
    
    FVector TargetPosition;
};

USTRUCT()
struct FSimEntityHealthChangedInfo
{
    GENERATED_BODY()
    
    FString Id;
    
    float HealthPercent;
};

USTRUCT()
struct FSimEntityDeadInfo
{
    GENERATED_BODY()
    
    FString Id;
};

USTRUCT()
struct FSimEntityStartMovementInfo
{
    GENERATED_BODY()
    
    FString Id;
    
    FVector CurrentPosition;
    FVector NewPosition;
    float MovementDuration;
};


USTRUCT()
struct FSimEntityFinishMovementInfo
{
    GENERATED_BODY()
    
    FString Id;
    
    FVector Position;
};

