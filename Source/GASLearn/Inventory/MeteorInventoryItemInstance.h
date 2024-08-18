// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeteorInventoryItemDefinition.h"
#include "MeteorInventoryItemInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASLEARN_API UMeteorInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UMeteorInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMeteorInventoryItemDefinition> ItemDefinition;

	UMeteorInventoryItemDefinition* GetItemDefinition() const { return ItemDefinition; }

	void SetItemDefinition(TObjectPtr<UMeteorInventoryItemDefinition> InItemDefinition) { ItemDefinition = InItemDefinition; }
	
	UPROPERTY()
	TArray<TObjectPtr<UMeteorInventoryFragmentBase>> DynamicFragmentInstances;

	UFUNCTION(BlueprintCallable, BlueprintPure="false", meta=(DeterminesOutputType=FragmentClass))
	const UMeteorInventoryFragmentBase* FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass) const;

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass(TSubclassOf<ResultClass> FragmentClass) const
	{
		return Cast<ResultClass>(FindFragmentByClass(FragmentClass));
	}
};
