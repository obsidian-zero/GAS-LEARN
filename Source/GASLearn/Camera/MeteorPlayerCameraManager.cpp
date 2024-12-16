// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/MeteorPlayerCameraManager.h"
#include "DrawDebugHelpers.h"
#include "GASLearn/Camera/MeteorCameraModeBase.h"
#include "GASLearn/Camera/MeteorCameraInfoBase.h"

AMeteorPlayerCameraManager::AMeteorPlayerCameraManager()
{
	
}

void AMeteorPlayerCameraManager::onPlayerControllerProcess_Implementation(APawn* InPawn)
{
	for(auto CameraInfo : CameraInfos)
	{
		if(IsValid(CameraInfo))
		{
			initCameraInfo(CameraInfo);
		}
	}
	for(auto CameraInfo: CameraInfos)
	{
		if(IsValid(CameraInfo) && CameraInfo->isCameraInfoValid())
		{
			CurrentCameraInfo = CameraInfo;
			CurrentCameraInfo->ActiveCameraInfo(this);
			break;
		}
	}
}

TObjectPtr<UMeteorCameraModeBase> AMeteorPlayerCameraManager::FindOrCreateCameraMode(TSubclassOf<UMeteorCameraModeBase> CameraModeClass)
{
	for (auto CameraMode : CameraModes)
	{
		if(CameraMode->IsA(CameraModeClass))
		{
			return CameraMode;
		}
	}
	TObjectPtr<UMeteorCameraModeBase> CameraModeInstance = NewObject<UMeteorCameraModeBase>(this, CameraModeClass);
	
	if (IsValid(CameraModeInstance))
	{
		CameraModeInstance->initCameraPlayerManager(this);
	}
	
	return CameraModeInstance;
}

void AMeteorPlayerCameraManager::initCameraInfo(TObjectPtr<UMeteorCameraInfoBase> & CameraInfo)
{
	if(IsValid(CameraInfo))
	{
		TObjectPtr<UMeteorCameraModeBase> CameraMode = FindOrCreateCameraMode(CameraInfo->TargetCameraModeClass);
		CameraInfo->TargetCameraMode = CameraMode;
	}
}

void AMeteorPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	if (OutVT.Target)
	{
		FVector OutLocation;
		FRotator OutRotation;
		float OutFOV;
		
		if (CustomCameraBehavior(DeltaTime, OutLocation, OutRotation, OutFOV))
		{
			OutVT.POV.Location = OutLocation;
			OutVT.POV.Rotation = OutRotation;
			OutVT.POV.FOV = OutFOV;
		}else if (BlueprintUpdateCamera(OutVT.Target, OutLocation, OutRotation, OutFOV))
		{
			OutVT.POV.Location = OutLocation;
			OutVT.POV.Rotation = OutRotation;
			OutVT.POV.FOV = OutFOV;
		}
		else
		{
			OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
		}
	}
}

bool AMeteorPlayerCameraManager::CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV)
{
	if(IsValid(CurrentCameraInfo) && CurrentCameraInfo->isCameraInfoValid())
	{
		return CurrentCameraInfo->TargetCameraMode->CustomCameraBehavior(CurrentCameraInfo, DeltaTime, Location, Rotation, FOV);
	}
	
	return false;
}