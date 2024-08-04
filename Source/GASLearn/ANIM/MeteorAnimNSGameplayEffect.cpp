// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/ANIM/MeteorAnimNSGameplayEffect.h"

UMeteorAnimNSGameplayEffect::UMeteorAnimNSGameplayEffect()
{
}

void UMeteorAnimNSGameplayEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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


void UMeteorAnimNSGameplayEffect::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();

}

FString UMeteorAnimNSGameplayEffect::GetNotifyName_Implementation() const
{
	FString DisplayName = TEXT("GameplayEffect");
	
	return DisplayName;
}

void UMeteorAnimNSGameplayEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			
			for(TSubclassOf<UGameplayEffect> GameplayEffect : StateEffects)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, AbilitySystemComponent, -1);
			}
		}
	}
}

#if WITH_EDITOR
bool UMeteorAnimNSGameplayEffect::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif // #if WITH_EDITOR