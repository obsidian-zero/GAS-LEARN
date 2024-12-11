// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/Camera/MeteorCameraModeBase.h"
#include "MeteorTPCameraMode.generated.h"

class UMeteorTPCameraInfo;
/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorTPCameraMode : public UMeteorCameraModeBase
{
	GENERATED_BODY()
	
public:
	virtual bool CustomCameraBehavior(UMeteorCameraInfoBase * CameraInfo, float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV) override;

	virtual bool isCameraModeValid(UMeteorCameraInfoBase * CameraInfo) override;

	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	virtual FVector PredictTPCameraTarget(UMeteorTPCameraInfo * TPCameraInfo);

	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	virtual void ModifyCameraByTrace(UMeteorTPCameraInfo * TPCameraInfo, FVector & TargetCameraLocation, FVector & PredictTPCameraLocation, float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	static FVector CalculateLocationLag(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);
};
