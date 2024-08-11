// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/ANIM/ComboAnimNotifyState.h"

void UComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}
	
	if(ADemoPlayerGASCharacterBase * Character = Cast<ADemoPlayerGASCharacterBase>(OwnerActor))
	{
		Character->ActiveActionGameplayAbilityComboInput(ActionGameplayAbility, ComboInputActions);
	}
}

FString UComboAnimNotifyState::GetNotifyName_Implementation() const
{
	
	return FString::Printf(TEXT("ComboGA: %s"), *ActionGameplayAbility->GetName());
}

void UComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}
	
	if(ADemoPlayerGASCharacterBase * Character = Cast<ADemoPlayerGASCharacterBase>(OwnerActor))
	{
		Character->DeActiveActionGameplayAbilityComboInput(ActionGameplayAbility, ComboInputActions);
	}
}