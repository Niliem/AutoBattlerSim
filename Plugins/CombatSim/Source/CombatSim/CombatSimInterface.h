// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatSimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatSimInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class COMBATSIM_API ICombatSimInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatSim")
    void UpdateHealth(float HealthPercent);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatSim")
    void Attack(FVector TargetPosition);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatSim")
    void Dead();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatSim")
    void StartMovement(FVector StartPosition, FVector TargetPosition, float Duration);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatSim")
    void FinishMovement(FVector TargetPosition);
};
