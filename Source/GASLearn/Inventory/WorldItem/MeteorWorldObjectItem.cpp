// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/WorldItem/MeteorWorldObjectItem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GASLearn/Inventory/Fragment/MeteorStaticMesh_FragmentBase.h"

// Sets default values
AMeteorWorldObjectItem::AMeteorWorldObjectItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default Mesh"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	RootComponent = StaticMeshComponent;

	InventoryInstance = nullptr;
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
		TSoftObjectPtr<UStaticMesh> Mesh = SMFragment->Mesh;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(Mesh.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Mesh]
		{
			if(Mesh.IsValid())
			{
				StaticMeshComponent->SetStaticMesh(Mesh.Get());
			}
		}));

	}
}

UMeteorInventoryItemInstance* AMeteorWorldObjectItem::GetInInventoryInstance()
{
	return InventoryInstance;
}
