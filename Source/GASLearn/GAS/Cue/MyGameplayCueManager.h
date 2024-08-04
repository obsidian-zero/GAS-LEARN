// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "MyGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMyGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	virtual void OnCreated() override;
};

