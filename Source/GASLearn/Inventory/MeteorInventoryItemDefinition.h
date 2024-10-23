// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeteorInventoryFragmentBase.h"
#include "Engine/DataAsset.h"
#include "MeteorInventoryItemDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GASLEARN_API UMeteorInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMeteorInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const TObjectPtr<UMeteorInventoryFragmentBase> FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass);
	
	template <typename T>
	T* FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass)
	{
		if (TObjectPtr<UMeteorInventoryFragmentBase> fragment = FindFragmentByClass(FragmentClass))
		{
			return Cast<T>(fragment);
		}
		return nullptr;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<TObjectPtr<UMeteorInventoryFragmentBase>> Fragments;
};
