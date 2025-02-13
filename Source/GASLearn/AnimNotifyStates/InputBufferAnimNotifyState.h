// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/AnimNotifyStates/MeteorAnimNotifyStateBase.h"
#include "InputBufferAnimNotifyState.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class GASLEARN_API UInputBufferAnimNotifyState : public UMeteorAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InputBuffer")
	TArray<UInputAction *> BufferActions;
};
