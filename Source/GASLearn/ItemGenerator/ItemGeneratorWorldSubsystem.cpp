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

AMeteorWorldObjectItem* UItemGeneratorWorldSubsystem::CreateWorldItemInstance(AMeteorLevelPlaceableItem * PlaceableItem)
{
	if (!IsValid(PlaceableItem))
	{
		return nullptr;
	}

	FTransform Transform = PlaceableItem->GetActorTransform();
	UMeteorInventoryBPFunctionLibrary::GenerateItemInstance(PlaceableItem->InventoryItemDefinition);
	return nullptr;
	// .get * ItemStack = ItemGenerator->GenerateItemStack(Context);
	//
	// AArcItemStackWorldObject* StackObject = GetWorld()->SpawnActorDeferred<AArcItemStackWorldObject>(GetDefault<UArcInventoryDeveloperSettings>()->ItemStackWorldObjectClass, Transform);
	// if (IsValid(StackObject))
	// {
	// 	StackObject->SetInventoryStack(ItemStack);
	// 	UGameplayStatics::FinishSpawningActor(StackObject, Transform);
	// 	SpawnedItemStack = StackObject;
	// 	return StackObject;
	// }
	// return *WorldItem;
}