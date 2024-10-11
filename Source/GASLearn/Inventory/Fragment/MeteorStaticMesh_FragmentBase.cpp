// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/Fragment/MeteorStaticMesh_FragmentBase.h"
#include "Net/UnrealNetwork.h"

void UMeteorStaticMesh_FragmentBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMeteorStaticMesh_FragmentBase, Mesh);
}