// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/Fragment/LevelStaticMesh_FragmentBase.h"
#include "Net/UnrealNetwork.h"

void ULevelStaticMesh_FragmentBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULevelStaticMesh_FragmentBase, Mesh);
}