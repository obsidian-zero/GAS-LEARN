// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/CameraModeInfos/MeteorTPCameraInfo.h"
#include "GASLearn/Camera/MeteorPlayerCameraManager.h"
#include "GameFramework/Character.h"

void UMeteorTPCameraInfo::GetTPPivotTarget(FTransform &TpCameraTarget)
{
	if (!IsValid(TargetActor))
	{
		return;
	}
	FVector Head = TargetActor->GetMesh()->GetSocketLocation(FName("Head"));
	FVector Root = TargetActor->GetMesh()->GetSocketLocation(FName("Root"));
	TpCameraTarget = FTransform(TargetActor->GetActorRotation(), (Head + Root) / 2.0f, FVector(1.0f));
	
}

void UMeteorTPCameraInfo::GetTPTraceParams(FVector& TraceOrigin, float& TraceRadius, TEnumAsByte<ECollisionChannel>& TraceChannel)
{
	if (!IsValid(TargetActor))
	{
		return;
	}
	if (TPTraceRightShoulder)
	{
		TraceOrigin = TargetActor->GetMesh()->GetSocketLocation(FName("TP_CameraTrace_R"));
	}else
	{
		TraceOrigin = TargetActor->GetMesh()->GetSocketLocation(FName("TP_CameraTrace_L"));
	}
	TraceRadius = TPTraceRadius;
	TraceChannel = TPTraceChannel;
}

bool UMeteorTPCameraInfo::ActiveCameraInfo(AMeteorPlayerCameraManager* CameraManager)
{
	TargetActor = Cast<ACharacter>(CameraManager->GetViewTarget());
	if (!IsValid(TargetActor))
	{
		return false;
	}
	return true;
}
