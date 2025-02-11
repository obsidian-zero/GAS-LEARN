// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "MeteorEnhancedPlayerInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEvaluateInputDelegates);
/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorEnhancedPlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()
	
	virtual void EvaluateInputDelegates(const TArray<UInputComponent*>& InputComponentStack, float DeltaTime, bool bGamePaused, const TArray<TPair<FKey, FKeyState*>>& KeysWithEvents) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnEvaluateInputDelegates OnBeforeEvaluateInputDelegates;

	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnEvaluateInputDelegates OnAfterEvaluateInputDelegates;
	
};
