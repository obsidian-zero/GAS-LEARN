// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/ANIM/MeteorAnimNotifyStateBase.h"
#include "ComboAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UComboAnimNotifyState : public UMeteorAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	FString GetNotifyName_Implementation() const override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combo")
	TArray<UInputAction *> ComboInputActions;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combo")
	TSubclassOf<UActionGameplayAbility> ActionGameplayAbility;
	
};
