// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemGeneratorWorldSubsystem.generated.h"

class AMeteorLevelPlaceableItem;
class AMeteorWorldObjectItem;
class UMeteorInventoryItemDefinition;
class UMeteorInventoryItemInstance;
/**
 * 
 */
UCLASS()
class GASLEARN_API UItemGeneratorWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	// 重写初始化方法
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 重写反初始化方法
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	AMeteorWorldObjectItem * CreateWorldItemInstanceByPlaceItem(AMeteorLevelPlaceableItem * PlaceableItem);

	UFUNCTION(BlueprintCallable)
	AMeteorWorldObjectItem* CreateWorldItemInstanceByDefinition(UMeteorInventoryItemDefinition* ItemDefinition, const FTransform& Transform);

	UFUNCTION(BlueprintCallable)
	AMeteorWorldObjectItem* CreateWorldItemInstanceForItemInstance(UMeteorInventoryItemInstance* ItemInstance, const FTransform& Transform);

	bool bIsGetPlaceInfo = false;
	void GetLevelRefreshActors(bool Force = false);
	TArray<AActor*> FoundActors;
};
