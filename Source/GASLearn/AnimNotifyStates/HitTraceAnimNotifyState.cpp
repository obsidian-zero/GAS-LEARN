// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/AnimNotifyStates/HitTraceAnimNotifyState.h"

void UHitTraceAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}
	
	if(UHitTraceComponent * HitTraceComponent = OwnerActor->FindComponentByClass<UHitTraceComponent>())
	{
		HitTraceComponent->StartHitTraceByName(TaskName);
	}
}

FString UHitTraceAnimNotifyState::GetNotifyName_Implementation() const
{
	
	return FString::Printf(TEXT("HitTrace: %s"), *TaskName);
}

void UHitTraceAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}
	
	if(UHitTraceComponent * HitTraceComponent = OwnerActor->FindComponentByClass<UHitTraceComponent>())
	{
		HitTraceComponent->EndHitTraceByName(TaskName);
	}
}