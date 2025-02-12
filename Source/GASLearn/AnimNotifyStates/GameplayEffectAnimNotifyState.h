// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/AnimNotifyStates/MeteorAnimNotifyStateBase.h"
#include "GameplayEffect.h"
#include "GASLearn/GAS/Character/Ability/CharacterAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UGameplayEffectAnimNotifyState : public UMeteorAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UGameplayEffectAnimNotifyState();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif // #if WITH_EDITOR

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meteor|Effect")
	TArray<TSubclassOf<class UGameplayEffect>> StateEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meteor")
	FString EffectName = "GameplayEffectANS";
};
