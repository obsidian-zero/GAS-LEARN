// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/TargetActors/AbilitySphereTargetActor.h"
#include "AbilitySystemComponent.h"

void AAbilitySphereTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	PrimaryPC = Cast<APlayerController>(Ability->GetAvatarActorFromActorInfo()->GetInstigatorController());
	SourceActor = Ability->GetAvatarActorFromActorInfo();
}

bool AAbilitySphereTargetActor::ShouldProduceTargetData() const
{
	// return true if we are locally owned, or (we are the server and this is a gameplaytarget ability that can produce target data server side)
	return (PrimaryPC && PrimaryPC->IsLocalController()) || ShouldProduceTargetDataOnServer;
}

void AAbilitySphereTargetActor::ConfirmTargetingAndContinue()
{
	if(!IsValid(SourceActor))
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	TArray<TWeakObjectPtr<AActor>> OverlapActors;

	//构建碰撞检测的params
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
	
	QueryParams.AddIgnoredActor(SourceActor->GetUniqueID());
	FVector AbilityStartLocation = SourceActor->GetActorLocation();

	//构建球形碰撞体，进行碰撞检测
	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		AbilityStartLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		QueryParams);

	DrawDebugSphere(
		GetWorld(),
		AbilityStartLocation,
		Radius,
		24, // 分段数，用于绘制球体的详细程度
		FColor::Green, // 球体颜色
		false, // 持续时间是否永久
		5.0f // 持续时间
	);
	//如果发生碰撞，将碰撞的actor写入OverlapActors
	if(TryOverlap)
	{
		
		for(const FOverlapResult& Result: OverlapResults)
		{
			APawn* OverlappedPawn = Cast<APawn>(Result.GetActor());
			if(IsValid(OverlappedPawn))
			{
				OverlapActors.AddUnique(OverlappedPawn);
			}
		}
	}

	//如果OverlapActors存在数据，将数据通过delegate广播
	if(OverlapActors.Num() > 0)
	{
		const FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapActors);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
}