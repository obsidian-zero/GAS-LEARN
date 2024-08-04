// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueTranslator.h"
#include "DamageTypeGameplayCueTranslator.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UDamageTypeGameplayCueTranslator : public UGameplayCueTranslator
{
	GENERATED_BODY()

	
public:

	// return (via out param) list of tag swaps you will do. This should be deterministic/order matters for later!
	virtual void GetTranslationNameSpawns(TArray<FGameplayCueTranslationNameSwap>& SwapList) const override;

	// runtime function to mapping Tag/Actor/Parameters to a translation index. The returned index here maps to the array was modified in ::GetTranslationNameSpawns
	virtual int32 GameplayCueToTranslationIndex(const FName& TagName, AActor* TargetActor, const FGameplayCueParameters& Parameters) const override;

	// Sorting priority. Higher number = first chance to translate a tag
	virtual int32 GetPriority() const override;

	// Whether this translator class should be enabled. Useful for disabling WIP translators
	virtual bool IsEnabled() const override;

	// Whether this translator should be shown in the top level view of the filter tree in the gameplaycue editor. If false, we will only add this as children of top level translators
	virtual bool ShouldShowInTopLevelFilterList() const override;
};
