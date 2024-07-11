// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/TargetActors/AbilityEyeObserverTargetActor.h"

AAbilityEyeObserverTargetActor::AAbilityEyeObserverTargetActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAbilityEyeObserverTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	PrimaryPC = Cast<APlayerController>(Ability->GetAvatarActorFromActorInfo()->GetInstigatorController());
	SourceActor = Ability->GetAvatarActorFromActorInfo();

	ConePoint = GetWorld()->SpawnActor<AActor>(ConePointClass, FVector::ZeroVector, FRotator::ZeroRotator);
}

bool AAbilityEyeObserverTargetActor::GetPlayerEyeObserverLocation(FVector& LookingPoint)
{
	// Get Player View Vector and View Rotation
	FVector ViewVector;
	FRotator ViewRotation;
	if(PrimaryPC)
		PrimaryPC->GetPlayerViewPoint(ViewVector, ViewRotation);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	if(SourceActor)
	{
		QueryParams.AddIgnoredActor(SourceActor->GetUniqueID());
	}
	
	FHitResult HitResult;
	
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ViewVector,
		ViewVector + ViewRotation.Vector() * Length,
		ECollisionChannel::ECC_WorldStatic,
		QueryParams);

	if(TryTrace)
	{
		LookingPoint = HitResult.ImpactPoint;
	}
	else
	{
		LookingPoint = FVector::ZeroVector;
	}

	return TryTrace;
}

void AAbilityEyeObserverTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector LookLocation;
	GetPlayerEyeObserverLocation(LookLocation);

	ConePoint->SetActorLocation(LookLocation);
}

void AAbilityEyeObserverTargetActor::ConfirmTargetingAndContinue()
{
	// ViewLocation是生成碰撞检测的位置
	FVector ViewLocation;
	GetPlayerEyeObserverLocation(ViewLocation);

	TArray<FOverlapResult> OverlapResults;
	TArray<TWeakObjectPtr<AActor>> OverlapActors;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
	if(SourceActor)
	{
		QueryParams.AddIgnoredActor(SourceActor->GetUniqueID());
	}

	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		ViewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		QueryParams);

	if(TryOverlap)
	{
		for(FOverlapResult& OverlapResult: OverlapResults)
		{
			APawn* PawnOverlapped = Cast<APawn>(OverlapResult.GetActor());
			if(PawnOverlapped)
			{
				OverlapActors.AddUnique(PawnOverlapped);
			}
		}
	}

	FVector MeteorSpawnLocation = ConePoint->GetActorLocation();
	MeteorSpawnLocation += ConePoint->GetActorUpVector() * 100.0f;
	EyeObserver = GetWorld()->SpawnActor<AActor>(EyeObserverClass, MeteorSpawnLocation, ConePoint->GetActorRotation());
	ConePoint->Destroy();

	OverlapActors.Add(EyeObserver);
	FGameplayAbilityTargetDataHandle TargetData =  StartLocation.MakeTargetDataHandleFromActors(OverlapActors);
	TargetDataReadyDelegate.Broadcast(TargetData);
	
}