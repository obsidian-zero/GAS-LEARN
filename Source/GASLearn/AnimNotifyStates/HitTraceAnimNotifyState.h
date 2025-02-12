// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/AnimNotifyStates/MeteorAnimNotifyStateBase.h"
#include "GASLearn/HitTraceComponent/HitTraceComponent.h"
#include "HitTraceAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UHitTraceAnimNotifyState : public UMeteorAnimNotifyStateBase
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere)
	FString TaskName;
};
