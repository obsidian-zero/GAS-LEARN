// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "MeteorInventoryItemInstance.h"
#include "MeteorInventoryFragmentBase.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Abstract)
class GASLEARN_API UMeteorInventoryFragmentBase : public UObject
{
	GENERATED_BODY()
public:
	// virtual void OnInstancedCreated(UMeteorInventoryItemInstance* Instance) const {};
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	bool bIsDynamic = false; //在ItemInstance中，是否转换成动态的Fragment，说明该Fragment是具备逻辑的且要保存状态的
	
};
