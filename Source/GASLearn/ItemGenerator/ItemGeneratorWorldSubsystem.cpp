// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/ItemGenerator/ItemGeneratorWorldSubsystem.h"
#include "GASLearn/Inventory/WorldItem/MeteorLevelPlaceableItem.h"
#include "GASLearn/Inventory/WorldItem/MeteorWorldObjectItem.h"
#include "GASLearn/Inventory/MeteorInventoryBPFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UItemGeneratorWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsGetPlaceInfo = false;
}

void UItemGeneratorWorldSubsystem::GetLevelRefreshActors(bool Force)
{
	if(!bIsGetPlaceInfo && !Force)
	{
		return;
	}
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeteorLevelPlaceableItem::StaticClass(), FoundActors);

	bIsGetPlaceInfo = true;
	// for (AActor* Actor : FoundActors)
	// {
	// 	if (Actor->IsA(AMeteorLevelPlaceableItem::StaticClass()))
	// 	{
	// 		Actor.RefreshTag
	// 	}
	// }
}

void UItemGeneratorWorldSubsystem::Deinitialize()
{
	// 清理资源或状态
	UE_LOG(LogTemp, Warning, TEXT("ItemGeneratorSubsystem Deinitialized!"));
	Super::Deinitialize();
}

AMeteorWorldObjectItem* UItemGeneratorWorldSubsystem::CreateWorldItemInstanceByPlaceItem(AMeteorLevelPlaceableItem * PlaceableItem)
{
	if (!IsValid(PlaceableItem))
	{
		return nullptr;
	}

	FTransform Transform = PlaceableItem->GetActorTransform();
	return CreateWorldItemInstanceByDefinition(PlaceableItem->InventoryItemDefinition, Transform);
}

AMeteorWorldObjectItem* UItemGeneratorWorldSubsystem::CreateWorldItemInstanceByDefinition(UMeteorInventoryItemDefinition* ItemDefinition, const FTransform& Transform)
{
	if (!IsValid(ItemDefinition))
	{
		return nullptr;
	}
	
	UMeteorInventoryItemInstance * ItemInstance = UMeteorInventoryBPFunctionLibrary::GenerateItemInstance(ItemDefinition);
	
	return CreateWorldItemInstanceForItemInstance(ItemInstance, Transform);
}

AMeteorWorldObjectItem* UItemGeneratorWorldSubsystem::CreateWorldItemInstanceForItemInstance(UMeteorInventoryItemInstance* ItemInstance, const FTransform& Transform)
{
	if (!IsValid(ItemInstance))
	{
		return nullptr;
	}

	AMeteorWorldObjectItem* WorldObject = GetWorld()->SpawnActorDeferred<AMeteorWorldObjectItem>(AMeteorWorldObjectItem::StaticClass(), Transform);
	if (IsValid(WorldObject))
	{
		WorldObject->SetInventoryInstance(ItemInstance);
		UGameplayStatics::FinishSpawningActor(WorldObject, Transform);
		return WorldObject;
	}
	return nullptr;
}