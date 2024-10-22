// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/WorldItem/MeteorWorldObjectItem.h"

#include "Engine/StreamableManager.h"
#include "GASLearn/Inventory/Fragment/MeteorStaticMesh_FragmentBase.h"

// Sets default values
AMeteorWorldObjectItem::AMeteorWorldObjectItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeteorWorldObjectItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeteorWorldObjectItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeteorWorldObjectItem::SetInventoryInstance_Implementation(UMeteorInventoryItemInstance* InInventoryInstance)
{
	InventoryInstance = InInventoryInstance;
	
	if (IsValid(InventoryInstance))
	{
		UMeteorInventoryItemInstance::TransferStackOwnership(InventoryInstance, this);
	}
	
	if (const UMeteorStaticMesh_FragmentBase* SMFragment = InventoryInstance->FindFragmentByClassType<UMeteorStaticMesh_FragmentBase>(UMeteorStaticMesh_FragmentBase::StaticClass()))
	{
		FStreamableManager Manager;
		TSoftObjectPtr<UStaticMesh> Mesh = SMFragment->Mesh;
		Manager.RequestAsyncLoad(SMFragment->Mesh.LoadSynchronous(), FStreamableDelegate::CreateWeakLambda(this, [this, Mesh]
		{
			StaticMeshComponent->SetStaticMesh(Mesh.LoadSynchronous());
		}));
	}
}

UMeteorInventoryItemInstance* AMeteorWorldObjectItem::GetInInventoryInstance()
{
	return InventoryInstance;
}
