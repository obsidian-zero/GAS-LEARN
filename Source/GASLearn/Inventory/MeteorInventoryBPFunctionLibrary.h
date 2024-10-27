// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeteorInventoryBPFunctionLibrary.generated.h"

class UMeteorInventoryItemInstance;
class UMeteorInventoryItemDefinition;
class AMeteorWorldObjectItem;
class ADemoPlayerGASCharacterBase;
class UMeteorInventoryComponent;


UCLASS()
class GASLEARN_API UMeteorInventoryBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Meteroy|Inventory", meta = (WorldContext = "WorldContextObject"))
	static bool SpawnWorldItem(UObject* WorldContextObject,  UMeteorInventoryItemInstance* ItemStack, const FTransform& Transform);
	
	UFUNCTION(BlueprintCallable, Category = "Meteroy|Inventory")
	static UMeteorInventoryItemInstance * GenerateItemInstance(UMeteorInventoryItemDefinition * ItemDefinition);

	UFUNCTION(BlueprintCallable, Category = "Meteroy|Inventory")
	static bool PlaceItemIntoInventory(AMeteorWorldObjectItem * WorldObject, ADemoPlayerGASCharacterBase * PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "Meteroy|Inventory")
	static AMeteorWorldObjectItem* DropItemIntoWorld(UMeteorInventoryComponent * InventoryComponent, const FMeteorInventoryItemSlotHandle & SlotHandle);
};
