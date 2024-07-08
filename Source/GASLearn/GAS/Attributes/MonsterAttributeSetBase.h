// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "MonsterAttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMonsterAttributeSetBase : public UCharacterAttributeSetBase
{
	GENERATED_BODY()
	
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
};


