// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeteorAnimNotifyStateBase.generated.h"

static FString MeteorAnimNotifyStateBaseName = "MeteorAnimNotifyStateBase";

/**
 * A base class for Meteor Anim Notify State
 */
UCLASS()
class GASLEARN_API UMeteorAnimNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meteor")
	FString NotifyName = MeteorAnimNotifyStateBaseName;
};
