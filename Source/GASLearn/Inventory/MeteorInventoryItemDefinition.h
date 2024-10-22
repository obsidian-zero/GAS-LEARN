// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeteorInventoryFragmentBase.h"
#include "Engine/DataAsset.h"
#include "MeteorInventoryItemDefinition.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASLEARN_API UMeteorInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMeteorInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UMeteorInventoryFragmentBase* FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass);

	TSubclassOf<UMeteorInventoryFragmentBase> FindDefaultFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass);

	template <typename T>
	T* FindDefaultFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass)
	{
		if (TSubclassOf<UMeteorInventoryFragmentBase> fragment = FindDefaultFragmentByClass(FragmentClass))
		{
			return Cast<T>(fragment.GetDefaultObject());
		}
		return nullptr;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<TObjectPtr<UMeteorInventoryFragmentBase>> Fragments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<UMeteorInventoryFragmentBase>> DefaultFragments;
};
