// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CombatSimDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Combat Sim Settings"))
class COMBATSIM_API UCombatSimDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
    
public:
    UPROPERTY(Config, EditAnywhere, Category = "Sim", meta = (ForceInlineRow, Categories = "Actors"))
    TMap<FString, TSubclassOf<AActor>> EntityClassesForType;
};
