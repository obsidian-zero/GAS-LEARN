// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"
#include "MeteorInventoryBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorInventoryBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Meteroy|Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool SpawnWorldItem(UObject* WorldContextObject,  UMeteorInventoryItemInstance* ItemStack, const FTransform& Transform);


};
