// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/MeteorInventoryComponent.h"

// Sets default values for this component's properties
UMeteorInventoryComponent::UMeteorInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeteorInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeteorInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UMeteorInventoryComponent::IsValidItemSlotIndex(const FMeteorInventoryItemSlotHandle & SlotHandle) const
{
	for(const auto & Slot : AllShotHandles)
	{
		if(Slot == SlotHandle)
		{
			return true;
		}
	}
	return false;
}

FMeteorInventoryItemSlot * UMeteorInventoryComponent::GetSlotByHandle(const FMeteorInventoryItemSlotHandle & SlotHandle)
{
	
	for(auto & Slot : InventorySlots.Slots)
	{
		if(SlotHandle == Slot)
		{
			return &Slot;
		}
	}
	return nullptr;
}

void UMeteorInventoryComponent::CreateInventorySlot()
{
	FMeteorInventoryItemSlot NewSlot;
	NewSlot.SlotIndex = SlotIndexCount++;
	NewSlot.Owner = this;
	
	InventorySlots.Slots.Add(NewSlot);

	PostInventoryUpdate();
}

void UMeteorInventoryComponent::RemoveInventorySlot(const FMeteorInventoryItemSlotHandle & SlotHandle)
{
	if(FMeteorInventoryItemSlot * Slot = GetSlotByHandle(SlotHandle))
	{
		InventorySlots.Slots.Remove(*Slot);
		PostInventoryUpdate();
	}
}

void UMeteorInventoryComponent::PostInventoryUpdate()
{
	AllShotHandles.Empty(AllShotHandles.Num() + 1);
	PopulateSlotReferenceArray(AllShotHandles);
}

void UMeteorInventoryComponent::PopulateSlotReferenceArray(TArray<FMeteorInventoryItemSlotHandle> & Handles)
{
	for(const auto & Slot : InventorySlots.Slots)
	{
		Handles.Add(FMeteorInventoryItemSlotHandle(Slot.SlotIndex, this));
	}
}

bool UMeteorInventoryComponent::PlaceItemIntoSlot(const FMeteorInventoryItemSlotHandle & SlotHandle, UMeteorInventoryItemInstance * ItemInstance)
{
	if(FMeteorInventoryItemSlot * Slot = GetSlotByHandle(SlotHandle))
	{
		Slot->ItemInstance = ItemInstance;
		return true;
	}
	return false;
}

bool UMeteorInventoryComponent::AddItemToEmptySlot(UMeteorInventoryItemInstance * ItemInstance)
{
	for(auto & Slot : InventorySlots.Slots)
	{
		if(Slot.ItemInstance == nullptr)
		{
			if (PlaceItemIntoSlot(FMeteorInventoryItemSlotHandle(Slot.SlotIndex, this), ItemInstance))
			{
				return true;
			}
		}
	}
	return false;
}

bool UMeteorInventoryComponent::RemoveItemFromSlot(const FMeteorInventoryItemSlotHandle & SlotHandle)
{
	if(FMeteorInventoryItemSlot * Slot = GetSlotByHandle(SlotHandle))
	{
		Slot->ItemInstance = nullptr;
		return true;
	}
	return false;
}

void UMeteorInventoryComponent::RemoveAllItemsFromSlot(TArray<UMeteorInventoryItemInstance *> & OutItemRemoved)
{
	OutItemRemoved.Empty(InventorySlots.Slots.Num());
	for(auto & Slot : InventorySlots.Slots)
	{
		if(Slot.ItemInstance != nullptr)
		{
			OutItemRemoved.Add(Slot.ItemInstance);
			RemoveItemFromSlot(FMeteorInventoryItemSlotHandle(Slot, this));
		}
	}
}

TArray<FMeteorInventoryItemSlotHandle> UMeteorInventoryComponent::GetAllItemSlots() const
{
	return AllShotHandles;
}

UMeteorInventoryItemInstance * UMeteorInventoryComponent::GetItemInstanceFromSlot(const FMeteorInventoryItemSlotHandle & SlotHandle)
{
	if(const FMeteorInventoryItemSlot * Slot = GetSlotByHandle(SlotHandle))
	{
		return Slot->ItemInstance;
	}
	return nullptr;
}