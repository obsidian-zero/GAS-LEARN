// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RageDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API URageDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	URageDamageExecution();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
