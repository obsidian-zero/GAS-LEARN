// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/MeteorInventoryBPFunctionLibrary.h"
#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"
#include "GASLearn/Inventory/MeteorInventoryItemDefinition.h"

bool UMeteorInventoryBPFunctionLibrary::SpawnWorldItem(UObject* WorldContextObject, UMeteorInventoryItemInstance* ItemStack, const FTransform& Transform)
{
	return true;
}

UMeteorInventoryItemInstance * UMeteorInventoryBPFunctionLibrary::GenerateItemInstance(UMeteorInventoryItemDefinition * ItemDefinition)
{
	TObjectPtr<UMeteorInventoryItemInstance> Instance = NewObject<UMeteorInventoryItemInstance>(GetTransientPackage(), UMeteorInventoryItemInstance::StaticClass());
	
	if (IsValid(Instance))
	{
		Instance->SetItemDefinition(ItemDefinition);
	}
	
	return Instance;
}