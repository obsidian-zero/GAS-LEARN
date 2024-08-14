// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"

const UMeteorInventoryFragmentBase* UMeteorInventoryItemInstance::FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass) const
{
	if(FragmentClass != nullptr)
	{
		if(ItemDefinition != nullptr)
		{
			return ItemDefinition->FindFragmentByClass(FragmentClass);
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