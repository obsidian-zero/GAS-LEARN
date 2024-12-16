// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MeteorPlayerCameraManager.generated.h"

class ADemoPlayerGASCharacterBase;
class UMeteorCameraBehavior;
class UMeteorCameraInfoBase;
class UMeteorCameraModeBase;
/**
 * 
 */
UCLASS()
class GASLEARN_API AMeteorPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	AMeteorPlayerCameraManager();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSystem")
	TArray<TObjectPtr<UMeteorCameraInfoBase>> CameraInfos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSystem")
	TArray<TObjectPtr<UMeteorCameraModeBase>> CameraModes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSystem")
	TObjectPtr<UMeteorCameraInfoBase> CurrentCameraInfo;

	TObjectPtr<UMeteorCameraModeBase> FindOrCreateCameraMode(TSubclassOf<UMeteorCameraModeBase> CameraModeClass);

	template<typename T>
	TObjectPtr<T> FindOrCreateCameraMode(TSubclassOf<T> CameraModeClass)
	{
		return Cast<T>(FindOrCreateCameraMode(CameraModeClass));
	}

	void initCameraInfo(TObjectPtr<UMeteorCameraInfoBase> & CameraInfo);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Camera")
	void onPlayerControllerProcess(APawn* InPawn);
	virtual void onPlayerControllerProcess_Implementation(APawn* InPawn);

	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);
};
