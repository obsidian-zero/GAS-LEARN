// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASLearn/GASLearn.h"
#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCharacterGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability");
	EDemoAbilityID AbilityInputID = EDemoAbilityID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability");
	EDemoAbilityID AbilityID = EDemoAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability");
	bool ActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
