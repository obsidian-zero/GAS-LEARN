// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/WorldItem/MeteorLevelPlaceableItem.h"

// Sets default values
AMeteorLevelPlaceableItem::AMeteorLevelPlaceableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	if (bSpawnItemOnBeginPlay)
	{
		SpawnWorldItem();
	}
}

// Called every frame
void AMeteorLevelPlaceableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMeteorLevelPlaceableItem::SpawnWorldItem()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return nullptr;
	}
	

	FTransform Transform = GetActorTransform();
	// FArcItemGeneratorContext Context;
	// UArcItemStackBase* ItemStack = ItemGenerator->GenerateItemStack(Context);
	//
	// AArcItemStackWorldObject* StackObject = GetWorld()->SpawnActorDeferred<AArcItemStackWorldObject>(GetDefault<UArcInventoryDeveloperSettings>()->ItemStackWorldObjectClass, Transform);
	// if (IsValid(StackObject))
	// {
	// 	StackObject->SetInventoryStack(ItemStack);
	// 	UGameplayStatics::FinishSpawningActor(StackObject, Transform);
	// 	SpawnedItemStack = StackObject;
	// 	return StackObject;
	// }


	return true;
}

void AMeteorLevelPlaceableItem::OnSpawnedItemStackConsumed(AActor* Actor)
{
	// if (Actor == SpawnedItemStack)
	// {
	// 	SpawnedItemStack->OnDestroyed.RemoveDynamic(this, &ThisClass::OnSpawnedItemStackConsumed);
	// 	SpawnedItemStack = nullptr;
	// }
}

#if WITH_EDITOR
void AMeteorLevelPlaceableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 检查 Property 是否有效
	if (PropertyChangedEvent.Property)
	{
		// 检查是否为 Transform 属性变化
		if (PropertyChangedEvent.Property->GetFName() == TEXT("RelativeLocation"))
		{
			FTransform CurrentTransform = GetActorTransform();
			UE_LOG(LogTemp, Warning, TEXT("Current Transform: %s"), *CurrentTransform.ToString());
		}
	}
	
	// if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AArcInvPlaceableItem, ItemGenerator) && IsValid(ItemGenerator))
	// {
	// 	FArcItemGeneratorContext Context;
	// 	UArcItemStackBase* ItemStack = ItemGenerator->GenerateItemStack(Context);
	//
	// 	if(UArcItemStackModular* MIS = Cast<UArcItemStackModular>(ItemStack))
	// 	{
	// 		if (IsValid(EditorStaticMesh))
	// 		{
	// 			if (UArcItemFragment_StaticMesh* SMFragment = MIS->FindFirstFragment<UArcItemFragment_StaticMesh>([](UArcItemFragment_StaticMesh* Fragment) -> bool {
	// 				return Fragment->FragmentTags.HasTag(FArcInvWorldItemMeshTag);
	// 				}))
	// 			{
	// 				TSoftObjectPtr<UStaticMesh> Mesh = SMFragment->Mesh;
	// 				UAssetManager::GetStreamableManager().RequestAsyncLoad(SMFragment->Mesh.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Mesh]
	// 				{
	// 					if(Mesh.IsValid())
	// 					{
	// 						EditorStaticMesh->SetStaticMesh(Mesh.Get());
	// 					}
	// 				}));
	// 			}
	// 		}
	// 	}
	// }
}
#endif