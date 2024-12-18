// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/Camera/MeteorCameraInfoBase.h"
#include "MeteorStaticFixedActorCameraInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASLEARN_API UMeteorStaticFixedActorCameraInfo : public UMeteorCameraInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSystem|Config")
	TObjectPtr<AActor> TargetActor;
	
	
	
};
