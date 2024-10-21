// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "MeteorInputTriggerCombo.generated.h"

class UEnhancedPlayerInput;
class UInputAction;
/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "MeteorCombo", NotInputConfigurable = "true"))
class GASLEARN_API UMeteorInputTriggerCombo : public UInputTriggerCombo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	float TotalTimeForUse = 2.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Trigger Settings")
	float CurrentTimeIncomboTotal = 0.0f;
	
	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
};
