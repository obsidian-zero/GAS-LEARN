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
	// 使用 Rename 方法直接修改所有权, 修改Outer后，能够绑定生命周期到新的Onwer上
	ItemInstance->Rename(nullptr, Owner);
    
	// // 递归处理子物品堆栈的所有权转移
	// for (FArcSubItemArrayEntry& SubStack : ItemStack->SubItemStacks.Items)
	// {
	// 	TransferStackOwnership(SubStack.SubItemStack, Owner);
	// }
 //    
	// // 标记数组为脏数据，以便触发必要的更新
	// ItemStack->SubItemStacks.MarkArrayDirty();
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
	}
}
