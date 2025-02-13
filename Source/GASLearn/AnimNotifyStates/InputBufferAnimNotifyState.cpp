// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/AnimNotifyStates/InputBufferAnimNotifyState.h"
#include "InputAction.h"
#include "GASLearn/EnhancedInput/MeteorAbilityInputComponent.h"

void UInputBufferAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}

	TObjectPtr<UMeteorAbilityInputComponent> AbilityInputComponent = OwnerActor->FindComponentByClass<UMeteorAbilityInputComponent>();	

	if(AbilityInputComponent)
	{
		AbilityInputComponent->StartInputBufferLock(BufferActions);
	}
}

void UInputBufferAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}

	TObjectPtr<UMeteorAbilityInputComponent> AbilityInputComponent = OwnerActor->FindComponentByClass<UMeteorAbilityInputComponent>();	

	if(AbilityInputComponent)
	{
		AbilityInputComponent->EndInputBufferLock(BufferActions);
	}
}