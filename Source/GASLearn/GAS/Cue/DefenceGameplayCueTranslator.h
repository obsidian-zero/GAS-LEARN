// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueTranslator.h"
#include "DefenceGameplayCueTranslator.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UDefenceGameplayCueTranslator : public UGameplayCueTranslator
{
	GENERATED_BODY()

public:
	// return (via out param) list of tag swaps you will do. This should be deterministic/order matters for later!
	virtual void GetTranslationNameSpawns(TArray<FGameplayCueTranslationNameSwap>& SwapList) const override;

	// runtime function to mapping Tag/Actor/Parameters to a translation index. The returned index here maps to the array was modified in ::GetTranslationNameSpawns
	virtual int32 GameplayCueToTranslationIndex(const FName& TagName, AActor* TargetActor, const FGameplayCueParameters& Parameters) const override;

};
