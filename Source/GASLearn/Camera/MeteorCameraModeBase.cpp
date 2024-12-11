// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/MeteorCameraModeBase.h"
#include "GASLearn/Camera/MeteorCameraInfoBase.h"



bool UMeteorCameraModeBase::CustomCameraBehavior(UMeteorCameraInfoBase * CameraInfo, float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV)
{
	return false;
}

bool UMeteorCameraModeBase::isCameraModeValid(UMeteorCameraInfoBase * CameraInfo)
{
	return true;
}