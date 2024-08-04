// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MyAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
public:
	virtual  UGameplayCueManager* GetGameplayCueManager() override;
};
