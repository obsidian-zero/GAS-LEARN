// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MeteorInventoryComponent.generated.h"


class UMeteorInventoryItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryComponentUpdate, class UMeteorInventoryComponent*, Inventory);

USTRUCT(BlueprintType)
struct FMeteorInventoryItemSlot
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Inventory")
	TObjectPtr<UMeteorInventoryItemInstance> ItemInstance;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	int32 SlotIndex;
	
	FMeteorInventoryItemSlot()
	:ItemInstance(nullptr), SlotIndex(INDEX_NONE)
	{
	}

	FMeteorInventoryItemSlot(const FMeteorInventoryItemSlot& InSlot)
	:ItemInstance(InSlot.ItemInstance), SlotIndex(InSlot.SlotIndex)
	{
	}

	bool operator==(const FMeteorInventoryItemSlot& Other) const
	{
		return ItemInstance == Other.ItemInstance && SlotIndex == Other.SlotIndex;
	}
	
	bool operator!=(const FMeteorInventoryItemSlot& Other) const
	{
		return !(*this == Other);
	}

	TObjectPtr<UMeteorInventoryComponent> Owner;
	
};


USTRUCT(BlueprintType)
struct FMeteorInventoryItemSlotArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FMeteorInventoryItemSlot> Slots;

	UPROPERTY()
	TObjectPtr<UMeteorInventoryComponent> Owner;
};

USTRUCT(BlueprintType)
struct FMeteorInventoryItemSlotHandle
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 SlotIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TObjectPtr<UMeteorInventoryComponent> ParentInventory;

	FMeteorInventoryItemSlotHandle()
	:SlotIndex(INDEX_NONE), ParentInventory(nullptr){}

	FMeteorInventoryItemSlotHandle(const FMeteorInventoryItemSlotHandle& InHandle)
	:SlotIndex(InHandle.SlotIndex), ParentInventory(InHandle.ParentInventory){}

	FMeteorInventoryItemSlotHandle(const FMeteorInventoryItemSlot& InSlot, UMeteorInventoryComponent* InParentInventory)
	:SlotIndex(InSlot.SlotIndex), ParentInventory(InParentInventory){}

	FMeteorInventoryItemSlotHandle(int32 SlotIndex, UMeteorInventoryComponent* InParentInventory)
	:SlotIndex(SlotIndex), ParentInventory(InParentInventory){}

	bool operator==(const FMeteorInventoryItemSlotHandle& Other) const
	{
		return SlotIndex == Other.SlotIndex && ParentInventory == Other.ParentInventory;
	}

	bool operator!=(const FMeteorInventoryItemSlotHandle& Other) const
	{
		return !(*this == Other);
	}

	bool operator ==(const FMeteorInventoryItemSlot& Other) const
	{
		return SlotIndex == Other.SlotIndex;
	}

	bool operator !=(const FMeteorInventoryItemSlot& Other) const
	{
		return !(*this == Other);
	}
};

UCLASS(BlueprintType, ClassGroup=(Inventory), Blueprintable, meta=(BlueprintSpawnableComponent) )
class GASLEARN_API UMeteorInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeteorInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	FMeteorInventoryItemSlotArray InventorySlots;

	TArray<FMeteorInventoryItemSlotHandle> AllShotHandles;

	int32 SlotIndexCount;

	bool IsValidItemSlotIndex(const FMeteorInventoryItemSlotHandle & SlotHandle) const;

	FMeteorInventoryItemSlot * GetSlotByHandle(const FMeteorInventoryItemSlotHandle & SlotHandle);

	UPROPERTY(BlueprintAssignable, Category = "Arc|Inventory")
	FOnInventoryComponentUpdate OnInventoryUpdate;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PostInventoryUpdate();

	void PopulateSlotReferenceArray(TArray<FMeteorInventoryItemSlotHandle> & Handles);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual void CreateInventorySlot();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual void RemoveInventorySlot(const FMeteorInventoryItemSlotHandle & SlotHandle);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool PlaceItemIntoSlot(const FMeteorInventoryItemSlotHandle & SlotHandle, UMeteorInventoryItemInstance * ItemInstance);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemToEmptySlot(UMeteorInventoryItemInstance * ItemInstance);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual UMeteorInventoryItemInstance * RemoveItemFromSlot(const FMeteorInventoryItemSlotHandle & SlotHandle);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual void RemoveAllItemsFromSlot(TArray<UMeteorInventoryItemInstance *> & OutItemRemoved);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	virtual TArray<FMeteorInventoryItemSlotHandle> GetAllItemSlots() const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual UMeteorInventoryItemInstance * GetItemInstanceFromSlot(const FMeteorInventoryItemSlotHandle & SlotHandle);
};
