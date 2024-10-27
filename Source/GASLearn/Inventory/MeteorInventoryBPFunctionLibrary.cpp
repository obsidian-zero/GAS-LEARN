// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/MeteorInventoryBPFunctionLibrary.h"
#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"
#include "GASLearn/Inventory/MeteorInventoryItemDefinition.h"
#include "GASLearn/Inventory/MeteorInventoryComponent.h"
#include "GASLearn/Inventory/WorldItem/MeteorWorldObjectItem.h"
#include "GASLearn/Public/Character/Player/DemoPlayerGASCharacterBase.h"
#include "GASLearn/ItemGenerator/ItemGeneratorWorldSubsystem.h"

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

bool UMeteorInventoryBPFunctionLibrary::PlaceItemIntoInventory(AMeteorWorldObjectItem * WorldObject, ADemoPlayerGASCharacterBase * PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return false; // PlayerCharacter 为空，返回 false
	}

	// 查找 MeteorInventoryComponent
	UMeteorInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UMeteorInventoryComponent>();

	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter does not have a MeteorInventoryComponent!"));
		return false; // 没有找到 MeteorInventoryComponent，返回 false
	}else{
		InventoryComponent->CreateInventorySlot();
		InventoryComponent->AddItemToEmptySlot(WorldObject->GetInInventoryInstance());
		WorldObject->Destroy();
		return true;
	}
}

AMeteorWorldObjectItem* UMeteorInventoryBPFunctionLibrary::DropItemIntoWorld(UMeteorInventoryComponent * InventoryComponent, const FMeteorInventoryItemSlotHandle & SlotHandle)
{
	if(!InventoryComponent)
	{
		return nullptr;
	}

	UWorld* World = InventoryComponent->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	AActor* Owner = InventoryComponent->GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	UItemGeneratorWorldSubsystem* ItemGeneratorSubsystem = World->GetSubsystem<UItemGeneratorWorldSubsystem>();
	if (!ItemGeneratorSubsystem)
	{
		return nullptr;
	}

	UMeteorInventoryItemInstance * ItemInstance = InventoryComponent->RemoveItemFromSlot(SlotHandle);

	return ItemGeneratorSubsystem->CreateWorldItemInstanceForItemInstance(ItemInstance, Owner->GetActorTransform());
}
