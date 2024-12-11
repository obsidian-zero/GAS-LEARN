// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MeteorCameraInfoBase.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorCameraInfoBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraSystem")
	bool isCameraInfoValid();
	
	
};
