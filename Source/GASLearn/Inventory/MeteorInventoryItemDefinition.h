// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeteorInventoryFragmentBase.h"
#include "Engine/DataAsset.h"
#include "MeteorInventoryItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UMeteorInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UMeteorInventoryFragmentBase* FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Inventory")
	TArray<TObjectPtr<UMeteorInventoryFragmentBase>> Fragments;
};
