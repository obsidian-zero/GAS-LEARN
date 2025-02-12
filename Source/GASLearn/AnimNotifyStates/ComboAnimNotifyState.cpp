// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/AnimNotifyStates/ComboAnimNotifyState.h"
#include "GASLearn/GAS/Ability/ActionGameplayAbility.h"
#include "GASLearn/EnhancedInput/MeteorAbilityInputComponent.h"

void UComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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
		AbilityInputComponent->ActiveActionGameplayAbilityComboInput(ActionGameplayAbility, ComboInputActions);
	}
}

FString UComboAnimNotifyState::GetNotifyName_Implementation() const
{
	if(NotifyName != MeteorAnimNotifyStateBaseName)
	{
		return NotifyName;
	}
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

	TObjectPtr<UMeteorAbilityInputComponent> AbilityInputComponent = OwnerActor->FindComponentByClass<UMeteorAbilityInputComponent>();	

	if(AbilityInputComponent)
	{
		AbilityInputComponent->DeActiveActionGameplayAbilityComboInput(ActionGameplayAbility, ComboInputActions);
	}
}