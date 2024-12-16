// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/CameraModes/MeteorTPCameraMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "GASLearn/Camera/CameraModeInfos/MeteorTPCameraInfo.h"
#include "GASLearn/Camera/MeteorPlayerCameraManager.h"
#include "DrawDebugHelpers.h"

FVector UMeteorTPCameraMode::CalculateLocationLag(FVector CurrentLocation, FVector TargetLocation,
															 FRotator CameraRotation, FVector LagSpeeds,
															 float DeltaTime)
{
	// CameraRotation.Roll = 0.0f;
	// CameraRotation.Pitch = 0.0f;
	// const FVector UnrotatedCurLoc = CameraRotation.UnrotateVector(CurrentLocation);
	// const FVector UnrotatedTargetLoc = CameraRotation.UnrotateVector(TargetLocation);

	return FVector(
		FMath::FInterpTo(CurrentLocation.X, TargetLocation.X, DeltaTime, LagSpeeds.X),
		FMath::FInterpTo(CurrentLocation.Y, TargetLocation.Y, DeltaTime, LagSpeeds.Y),
		FMath::FInterpTo(CurrentLocation.Z, TargetLocation.Z, DeltaTime, LagSpeeds.Z));

	// return CameraRotation.RotateVector(ResultVector);
}


bool UMeteorTPCameraMode::isCameraModeValid(UMeteorCameraInfoBase * CameraInfo)
{
	if(!IsValid(CameraInfo)
		|| !CameraInfo->isCameraInfoValid()
		|| !CameraInfo->IsA<UMeteorTPCameraInfo>()
		|| !IsValid(CameraManager))
	{
		return false;
	}
	return true;
}

FVector UMeteorTPCameraMode::PredictTPCameraTarget(UMeteorTPCameraInfo * TPCameraInfo)
{
	
	FTransform PivotTarget;
	TPCameraInfo->GetTPPivotTarget(PivotTarget);
	
	// 获取 Controller 的旋转作为目标相机旋转
	FRotator PredictTargetTPCameraRotation = CameraManager->GetOwningPlayerController()->GetControlRotation();

	// 计算枢轴位置
	FVector PredictPivotLocation = 
		PivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(PivotTarget.Rotator()) * TPCameraInfo->PivotOffsetX +
		UKismetMathLibrary::GetRightVector(PivotTarget.Rotator()) * TPCameraInfo->PivotOffsetY +
		UKismetMathLibrary::GetUpVector(PivotTarget.Rotator()) * TPCameraInfo->PivotOffsetZ;

	// 计算相机位置
	FVector PredictTargetCameraLocation = 
		PredictPivotLocation +
		UKismetMathLibrary::GetForwardVector(PredictTargetTPCameraRotation) * TPCameraInfo->CameraOffsetX +
		UKismetMathLibrary::GetRightVector(PredictTargetTPCameraRotation) * TPCameraInfo->CameraOffsetY +
		UKismetMathLibrary::GetUpVector(PredictTargetTPCameraRotation) * TPCameraInfo->CameraOffsetZ;

	if(DrawDebugInfo)
	{
		DrawDebugSphere(GetWorld(), PredictTargetCameraLocation, 10.0f, 36, FColor::Blue, false, 0.1f, 0, 1.0f);
	}
	
	return PredictTargetCameraLocation;
}

void UMeteorTPCameraMode::ModifyCameraByTrace(UMeteorTPCameraInfo * TPCameraInfo, FVector & TargetCameraLocation, FVector & PredictTPCameraLocation, float DeltaTime)
{
	FVector TraceOrigin;
	float TraceRadius;
	TEnumAsByte<ECollisionChannel> TraceChannel;
	TPCameraInfo->GetTPTraceParams(TraceOrigin, TraceRadius, TraceChannel);

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CameraManager);
	Params.AddIgnoredActor(static_cast<const AActor*>(TPCameraInfo->TargetActor));

	FHitResult HitResult;
	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceRadius);
	const bool bHit = World->SweepSingleByChannel(HitResult, TraceOrigin, TargetCameraLocation, FQuat::Identity,
												  TraceChannel, SphereCollisionShape, Params);

	float dis = 0.0f;
	if (HitResult.IsValidBlockingHit())
	{
		TargetCameraLocation += HitResult.Location - HitResult.TraceEnd;
	}

	dis = (TargetCameraLocation - TraceOrigin).Length();

	FHitResult PredictHitResult;
	const bool predictHit = World->SweepSingleByChannel(PredictHitResult, TraceOrigin, PredictTPCameraLocation, FQuat::Identity,
												  TraceChannel, SphereCollisionShape, Params);
	
	float predictDistance = TPCameraInfo->PreviousCameraSpringLength;
	if (PredictHitResult.IsValidBlockingHit())
	{
		PredictTPCameraLocation += PredictHitResult.Location - PredictHitResult.TraceEnd;
		predictDistance = (PredictTPCameraLocation - TraceOrigin).Length();
	}
	if(predictHit && dis - predictDistance > TPCameraInfo->TracePredictRetractionThreshold)
	{
		FVector AdjustDirection = (TraceOrigin - TargetCameraLocation).GetSafeNormal(); // 计算调整方向
		FVector AdjustedLocation = TargetCameraLocation + AdjustDirection * (dis - predictDistance);
		FVector NowLocation = TargetCameraLocation + AdjustDirection * (dis - TPCameraInfo->PreviousCameraSpringLength);
		
		float alpha = DeltaTime * TPCameraInfo->TracePredictRetractionSpeed;
		TargetCameraLocation = FMath::Lerp(NowLocation, AdjustedLocation, alpha);
		
	}else if (dis - TPCameraInfo->PreviousCameraSpringLength > TPCameraInfo->TraceIncreaseLagThreshold || (TPCameraInfo->InTraceLag && dis > TPCameraInfo->PreviousCameraSpringLength))
	{
		TPCameraInfo->InTraceLag = true;
		float alpha = DeltaTime * TPCameraInfo->TraceIncreaseLagSpeed;
		
		FVector Direction = (TraceOrigin - TargetCameraLocation).GetSafeNormal(); // 计算朝向

		FVector NewTargetLocation = TargetCameraLocation + Direction * (dis - TPCameraInfo->PreviousCameraSpringLength);
		TargetCameraLocation = FMath::Lerp(NewTargetLocation, TargetCameraLocation, alpha);
	}else
	{
		TPCameraInfo->InTraceLag = false;
	}
	
	TPCameraInfo->PreviousCameraSpringLength = (TargetCameraLocation - TraceOrigin).Length();
}

bool UMeteorTPCameraMode::CustomCameraBehavior(UMeteorCameraInfoBase * CameraInfo, float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV)
{

	TObjectPtr<UMeteorTPCameraInfo> TPCameraInfo = Cast<UMeteorTPCameraInfo>(CameraInfo);
	
	FTransform PivotTarget;
	TPCameraInfo->GetTPPivotTarget(PivotTarget);
	
	FRotator TargetCameraRotation = FMath::RInterpTo(CameraManager->GetCameraRotation(),
													CameraManager->GetOwningPlayerController()->GetControlRotation(),
													DeltaTime,
													TPCameraInfo->LagRotatorSpeed);

	
	FVector LagSpeed(
		TPCameraInfo->LagPivotSpeedX,
		TPCameraInfo->LagPivotSpeedY,
		TPCameraInfo->LagPivotSpeedZ
		);
	
	const FVector& AxisIndpLag = CalculateLocationLag(TPCameraInfo->SmoothedPivotTarget.GetLocation(),
															 PivotTarget.GetLocation(),
															 TargetCameraRotation,
															 LagSpeed,
															 DeltaTime);
	
	TPCameraInfo->SmoothedPivotTarget.SetLocation(AxisIndpLag);
	TPCameraInfo->SmoothedPivotTarget.SetRotation(PivotTarget.GetRotation());
	TPCameraInfo->SmoothedPivotTarget.SetScale3D(FVector::OneVector);
	
	if(DrawDebugInfo)
	{
		DrawDebugSphere(GetWorld(), AxisIndpLag, 10.0f, 36, FColor::Orange, false, 0.1f, 0, 1.0f);
	}
	
	 FVector PivotLocation =
		TPCameraInfo->SmoothedPivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(TPCameraInfo->SmoothedPivotTarget.Rotator()) * TPCameraInfo->PivotOffsetX
		+UKismetMathLibrary::GetRightVector(TPCameraInfo->SmoothedPivotTarget.Rotator()) * TPCameraInfo->PivotOffsetY
		+UKismetMathLibrary::GetUpVector(TPCameraInfo->SmoothedPivotTarget.Rotator()) * TPCameraInfo->PivotOffsetZ;

	if(DrawDebugInfo)
	{
		DrawDebugSphere(GetWorld(), PivotLocation, 10.0f, 36, FColor::Red, false, 0.1f, 0, 1.0f);
	}
	
	FVector TargetCameraLocation = PivotLocation
		+UKismetMathLibrary::GetForwardVector(TargetCameraRotation) * TPCameraInfo->CameraOffsetX
		+UKismetMathLibrary::GetRightVector(TargetCameraRotation) * TPCameraInfo->CameraOffsetY
		+UKismetMathLibrary::GetUpVector(TargetCameraRotation) * TPCameraInfo->CameraOffsetZ;

	if(DrawDebugInfo)
	{
		DrawDebugSphere(GetWorld(), TargetCameraLocation, 10.0f, 36, FColor::Yellow, false, 0.1f, 0, 1.0f);
	}
	
	FVector PredictTPCameraLocation = PredictTPCameraTarget(TPCameraInfo);

	
	Location = AxisIndpLag;
	Rotation = PivotTarget.GetRotation().Rotator();
	
	ModifyCameraByTrace(TPCameraInfo, TargetCameraLocation, PredictTPCameraLocation, DeltaTime);

	if(DrawDebugInfo)
	{
		DrawDebugSphere(GetWorld(), TargetCameraLocation, 10.0f, 36, FColor::Green, false, 0.1f, 0, 1.0f);
	}

	TPCameraInfo->TargetCameraLocation = TargetCameraLocation;
	
	Location = TargetCameraLocation;
	Rotation = TargetCameraRotation;
	FOV = TPCameraInfo->TPFov;
	return true;
}

