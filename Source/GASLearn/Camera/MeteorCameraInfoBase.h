// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "MeteorCameraInfoBase.generated.h"

class UMeteorCameraModeBase;
class AMeteorPlayerCameraManager;
/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class GASLEARN_API UMeteorCameraInfoBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	bool isCameraInfoValid();

	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	virtual bool ActiveCameraInfo(AMeteorPlayerCameraManager* CameraManager);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraSystem|Info")
	TObjectPtr<UMeteorCameraModeBase> TargetCameraMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem|Info")
	TSubclassOf<UMeteorCameraModeBase> TargetCameraModeClass;
	
};
