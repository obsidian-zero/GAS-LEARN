// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/MeteorInventoryItemDefinition.h"

UMeteorInventoryItemDefinition::UMeteorInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UMeteorInventoryFragmentBase* UMeteorInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UMeteorInventoryFragmentBase> FragmentClass)
{
	if(FragmentClass != nullptr)
	{
		for(UMeteorInventoryFragmentBase *Fragment : Fragments)
		{
			if(Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	
	return nullptr;
}