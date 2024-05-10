// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GameplayEffect.h"
#include "RageHurtCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API URageHurtCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	URageHurtCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition RageSelfDef;

	FGameplayEffectAttributeCaptureDefinition RageEnermyDef;
};
