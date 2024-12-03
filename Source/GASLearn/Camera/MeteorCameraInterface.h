// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/EngineTypes.h"
#include "MeteorCameraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMeteorCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASLEARN_API IMeteorCameraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraSystem")
	void GetCameraParam(float& TpFov, float& FpFov, bool& RightShoulder);
	virtual void GetCameraParam_Implementation(float& TpFov, float& FpFov, bool& RightShoulder) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraSystem")
	void GetFPCameraTarget(FVector &FpCameraTarget);
	virtual void GetFPCameraTarget_Implementation(FVector &FpCameraTarget) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraSystem")
	void GetTPPivotTarget(FTransform &TpCameraTarget);
	virtual void GetTPPivotTarget_Implementation(FTransform &TpCameraTarget) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraSystem")
	void GetTPTraceParams(FVector &TraceOrigin, float &TraceRadius, TEnumAsByte<ECollisionChannel> &TraceChannel);
	virtual void GetTPTraceParams_Implementation(FVector &TraceOrigin, float &TraceRadius, TEnumAsByte<ECollisionChannel> &TraceChannel) = 0;
	
};
