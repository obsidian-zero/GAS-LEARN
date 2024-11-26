// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Engine/OverlapResult.h"
#include "AbilityEyeObserverTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API AAbilityEyeObserverTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AAbilityEyeObserverTargetActor();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSetting", meta = (ExposeOnSpawn = true))
	float Radius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSetting", meta = (ExposeOnSpawn = true))
	float Length = 10000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSetting", meta = (ExposeOnSpawn = true))
	TSubclassOf<AActor> ConePointClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSetting", meta = (ExposeOnSpawn = true))
	TSubclassOf<AActor> EyeObserverClass;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool GetPlayerEyeObserverLocation(FVector& OutLocation);

protected:
	AActor *  ConePoint;
	
	AActor *  EyeObserver;

};
