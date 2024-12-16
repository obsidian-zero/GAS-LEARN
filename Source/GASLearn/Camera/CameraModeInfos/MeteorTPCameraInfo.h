// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/Camera/MeteorCameraInfoBase.h"
#include "MeteorTPCameraInfo.generated.h"

class UMeteorTPCameraMode;

/**
 * 
 */
UCLASS(Blueprintable)
class GASLEARN_API UMeteorTPCameraInfo : public UMeteorCameraInfoBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	void GetTPPivotTarget(FTransform &TpCameraTarget);

	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	void GetTPTraceParams(FVector &TraceOrigin, float &TraceRadius, TEnumAsByte<ECollisionChannel> &TraceChannel);

	virtual bool ActiveCameraInfo(AMeteorPlayerCameraManager* CameraManager) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraSystem|Info")
	FTransform SmoothedPivotTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraSystem|Info")
	FVector TargetCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraSystem|Info")
	float PreviousCameraSpringLength = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraSystem|Info")
	bool InTraceLag = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSystem|Info")
	TObjectPtr<ACharacter> TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float TPFov = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	bool TPTraceRightShoulder = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float TPTraceRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	TEnumAsByte<ECollisionChannel> TPTraceChannel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float LagRotatorSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float LagPivotSpeedX = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float LagPivotSpeedY = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float LagPivotSpeedZ = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float PivotOffsetX = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float PivotOffsetY = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float PivotOffsetZ = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float CameraOffsetX = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float CameraOffsetY = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float CameraOffsetZ = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float TraceIncreaseLagThreshold = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float TraceIncreaseLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float TracePredictRetractionThreshold = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Config")
	float TracePredictRetractionSpeed = 0.0f;
};
