// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "K2Node_CallFunction.h"
#include "MeteorGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	static void CollectAllGameplayEffects(UObject* GameplayAbility, TArray<TSubclassOf<UGameplayEffect>>& OutEffects);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
