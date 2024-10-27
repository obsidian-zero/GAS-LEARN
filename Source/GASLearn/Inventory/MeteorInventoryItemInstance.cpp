// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"
#include "GASLearn/Inventory/MeteorInventoryFragmentBase.h"
#include "GASLearn/Inventory/MeteorInventoryItemDefinition.h"

UMeteorInventoryItemInstance::UMeteorInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UMeteorInventoryFragmentBase* UMeteorInventoryItemInstance::FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass) const
{
	if(FragmentClass != nullptr)
	{
		if(ItemDefinition != nullptr)
		{
			if(UMeteorInventoryFragmentBase * Fragment = ItemDefinition->FindFragmentByClass(FragmentClass))
			{
				return Fragment;
			}
		}

		for (TObjectPtr<UMeteorInventoryFragmentBase> Fragment : DynamicFragmentInstances)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

void UMeteorInventoryItemInstance::TransferStackOwnership(UMeteorInventoryItemInstance*& ItemInstance, AActor* Owner)
{

	AActor* OldOwner = Cast<AActor>(ItemInstance->GetOuter());
	// 使用 Rename 方法直接修改所有权, 修改Outer后，能够绑定生命周期到新的Onwer
	ItemInstance->Rename(nullptr, Owner);

	ItemInstance->OnItemInstanceOwnerChanged.Broadcast(ItemInstance, OldOwner, Owner);
}

void UMeteorInventoryItemInstance::SetItemDefinition(TObjectPtr<UMeteorInventoryItemDefinition> InItemDefinition)
{
	ItemDefinition = InItemDefinition;
	DynamicFragmentInstances.Reset();

	for(TObjectPtr<UMeteorInventoryFragmentBase> Fragment: ItemDefinition->Fragments)
	{
		if(Fragment->bIsDynamic)
		{
			UMeteorInventoryFragmentBase* DynamicFragment = DuplicateObject<UMeteorInventoryFragmentBase>(Fragment, this);

			DynamicFragmentInstances.Add(DynamicFragment);
		}
		Fragment->onItemInstancedCreated(this);
	}
}
