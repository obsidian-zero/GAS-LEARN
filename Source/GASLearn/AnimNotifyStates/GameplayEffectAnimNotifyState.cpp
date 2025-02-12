// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/AnimNotifyStates/GameplayEffectAnimNotifyState.h"

UGameplayEffectAnimNotifyState::UGameplayEffectAnimNotifyState()
{
}

void UGameplayEffectAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}
	
	// 使用 IAbilitySystemInterface 接口检查 Actor 是否具备 AbilitySystemComponent
	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerActor))
	{
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(OwnerActor);

			for(TSubclassOf<UGameplayEffect> GameplayEffect : StateEffects)
			{
				FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle);
				if (NewHandle.IsValid())
				{
					AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
				}
			}
		}
	}
}


void UGameplayEffectAnimNotifyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
}

void UGameplayEffectAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meteor AnimNS: OwnerActor is NULL"));
		return;
	}

	// 使用 IAbilitySystemInterface 接口检查 Actor 是否具备 AbilitySystemComponent
	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerActor))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			for(TSubclassOf<UGameplayEffect> GameplayEffect : StateEffects)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, AbilitySystemComponent, -1);
			}
		}
	}
}

#if WITH_EDITOR
bool UGameplayEffectAnimNotifyState::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif // #if WITH_EDITOR