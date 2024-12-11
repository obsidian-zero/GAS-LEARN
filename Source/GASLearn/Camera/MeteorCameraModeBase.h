// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MeteorCameraModeBase.generated.h"

class UMeteorCameraInfoBase;
/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorCameraModeBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	virtual bool CustomCameraBehavior(UMeteorCameraInfoBase * CameraInfo, float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);

	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	virtual bool isCameraModeValid(UMeteorCameraInfoBase * CameraInfo);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSystem")
	TObjectPtr<APlayerCameraManager> CameraManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSystem|Debug")
	bool DrawDebugInfo = false;
};
