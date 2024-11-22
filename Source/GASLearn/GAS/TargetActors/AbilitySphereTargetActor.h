// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "AbilitySphereTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API AAbilitySphereTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual bool ShouldProduceTargetData() const;

	virtual void ConfirmTargetingAndContinue() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSetting", meta = (ExposeOnSpawn = true))
	float Radius = 500.0f;
};
