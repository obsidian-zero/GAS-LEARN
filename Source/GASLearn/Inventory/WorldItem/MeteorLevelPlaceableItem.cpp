// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/WorldItem/MeteorLevelPlaceableItem.h"

#include "GASLearn/Inventory/Fragment/LevelStaticMesh_FragmentBase.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GASLearn/Inventory/Fragment/MeteorStaticMesh_FragmentBase.h"

// Sets default values
AMeteorLevelPlaceableItem::AMeteorLevelPlaceableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
#if WITH_EDITOR
	EditorStaticMesh = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("EditorVis"));
	if (IsValid(EditorStaticMesh))
	{
		EditorStaticMesh->SetupAttachment(RootComponent);
		EditorStaticMesh->bIsEditorOnly = true;
		EditorStaticMesh->bHiddenInGame = true;
		EditorStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
#endif
}

// Called when the game starts or when spawned
void AMeteorLevelPlaceableItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMeteorLevelPlaceableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void AMeteorLevelPlaceableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//关卡物品摆设时，根据道具定义尝试自动加载mesh
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AMeteorLevelPlaceableItem, InventoryItemDefinition) && IsValid(InventoryItemDefinition))
	{
		if (EditorStaticMesh)
		{
			// 先尝试找到专门的关卡展示fragment
			if (TSubclassOf<ULevelStaticMesh_FragmentBase> levelFragement = InventoryItemDefinition.GetDefaultObject()->FindDefaultFragmentByClass(ULevelStaticMesh_FragmentBase::StaticClass()))
			{
				
				TSoftObjectPtr<UStaticMesh> Mesh = levelFragement.GetDefaultObject()->Mesh;
				UAssetManager::GetStreamableManager().RequestAsyncLoad(Mesh.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Mesh]
				{
					if(Mesh.IsValid())
					{
						EditorStaticMesh->SetStaticMesh(Mesh.Get());
					}
				}));
			
			}else
			{
				// 如果没有专门的关卡展示fragment，就找普通的mesh fragment
				if(TSubclassOf<UMeteorStaticMesh_FragmentBase> staticMeshFragment = InventoryItemDefinition.GetDefaultObject()->FindDefaultFragmentByClass(UMeteorStaticMesh_FragmentBase::StaticClass()))
				{
				
					TSoftObjectPtr<UStaticMesh> Mesh = staticMeshFragment.GetDefaultObject()->Mesh;
					UAssetManager::GetStreamableManager().RequestAsyncLoad(Mesh.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Mesh]
					{
						if(Mesh.IsValid())
						{
							EditorStaticMesh->SetStaticMesh(Mesh.Get());
						}
					}));
				
				}
			}
		}
	}
}
#endif