// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/GAS/Character/Ability/CharacterAbilitySystemComponent.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "GASLearn/GAS/Attributes/MonsterAttributeSetBase.h"
#include "Character/GASCharacterBase.h"
#include "EntityCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API AEntityCharacterBase : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	AEntityCharacterBase();

	AEntityCharacterBase(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY()
	class UCharacterAbilitySystemComponent * EntityAbilitySystemComponent;

	UPROPERTY()
	class UCharacterAttributeSetBase * EntityAttributeSetBase;

	void BeginPlay() override;
	
};
