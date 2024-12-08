// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/MeteorPlayerCameraManager.h"
#include "GASLearn/Camera/MeteorCameraInterface.h"
#include "GASLearn/Public/Character/Player/DemoPlayerGASCharacterBase.h"
#include "GASLearn/Camera/MeteorCameraBehavior.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

AMeteorPlayerCameraManager::AMeteorPlayerCameraManager()
{
	CameraMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraMesh"));
	CameraMesh->SetupAttachment(GetRootComponent());
	CameraMesh->bHiddenInGame = true;
}

void AMeteorPlayerCameraManager::onPlayerControllerProcess_Implementation(APawn* InPawn)
{
	// PlayerCameraManager需要知道当前控制的Pawn是什么，所以我们需要在这里保存一下
	if(ControlledPawn = Cast<ADemoPlayerGASCharacterBase>(InPawn))
	{
		
		ControlledPawn->Execute_GetTPPivotTarget(ControlledPawn, SmoothedPivotTarget);
		SetActorLocation(SmoothedPivotTarget.GetLocation());
	}

	if(IsValid(CameraMesh))
	{
		if(CameraBehavior = Cast<UMeteorCameraBehavior>(CameraMesh->GetAnimInstance()))
		{
			CameraBehavior->PlayerController = Cast<APlayerController>(InPawn->Controller);
			CameraBehavior->PlayerPawn = InPawn;
		}
	}
}

float AMeteorPlayerCameraManager::GetAnimCurveValue(FName CurveName) const
{
	if(IsValid(CameraBehavior))
	{
		return CameraBehavior->GetCurveValue(CurveName);
	}
	return 0.0f;
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

FVector AMeteorPlayerCameraManager::CalculateLocationLagInCamera(FVector CurrentLocation, FVector TargetLocation,
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

FVector AMeteorPlayerCameraManager::PredictTPCameraTarget()
{
	// 假定 PivotTarget 是从 ControlledPawn 的接口获取到的
	FTransform PivotTarget;
	ControlledPawn->Execute_GetTPPivotTarget(ControlledPawn, PivotTarget);

	// 假定这些偏移曲线值已经在某处定义好（比如从动画曲线中获取或固定值）
	float PivotOffsetX = GetAnimCurveValue(PivotOffsetCurveX);
	float PivotOffsetY = GetAnimCurveValue(PivotOffsetCurveY);
	float PivotOffsetZ = GetAnimCurveValue(PivotOffsetCurveZ);

	float CameraOffsetX = GetAnimCurveValue(CameraOffsetCurveX);
	float CameraOffsetY = GetAnimCurveValue(CameraOffsetCurveY);
	float CameraOffsetZ = GetAnimCurveValue(CameraOffsetCurveZ);

	// 获取 Controller 的旋转作为目标相机旋转
	FRotator PredictTargetTPCameraRotation = GetOwningPlayerController()->GetControlRotation();

	// 计算枢轴位置
	FVector PredictPivotLocation = 
		PivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(PivotTarget.Rotator()) * PivotOffsetX +
		UKismetMathLibrary::GetRightVector(PivotTarget.Rotator()) * PivotOffsetY +
		UKismetMathLibrary::GetUpVector(PivotTarget.Rotator()) * PivotOffsetZ;

	// 计算相机位置
	FVector PredictTargetCameraLocation = 
		PredictPivotLocation +
		UKismetMathLibrary::GetForwardVector(PredictTargetTPCameraRotation) * CameraOffsetX +
		UKismetMathLibrary::GetRightVector(PredictTargetTPCameraRotation) * CameraOffsetY +
		UKismetMathLibrary::GetUpVector(PredictTargetTPCameraRotation) * CameraOffsetZ;

	return PredictTargetCameraLocation;
}

bool AMeteorPlayerCameraManager::CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV)
{
	if(!IsValid(ControlledPawn))
	{
		return false;
	}

	// 第一步，通过接口获取到我们需要的摄像机上的一些信息
	FTransform PivotTarget;
	ControlledPawn->Execute_GetTPPivotTarget(ControlledPawn, PivotTarget);

	FVector FPTarget;
	ControlledPawn->Execute_GetFPCameraTarget(ControlledPawn, FPTarget);
	float TPFOV = 90.0f;
	float FPFOV = 90.0f;
	bool bRightShoulder = false;
	ControlledPawn->Execute_GetCameraParam(ControlledPawn, TPFOV, FPFOV, bRightShoulder);

	// 第二步，计算摄像机的旋转, 通过曲线反应的数据来混合具体的旋转

	// 摄像头最终的目标是Controller上面的旋转，曲线进行差值
	const FRotator& InterpResult = FMath::RInterpTo(GetCameraRotation(),
													GetOwningPlayerController()->GetControlRotation(),
													DeltaTime,
													GetAnimCurveValue(LagRotatorSpeedCurve));

	// 混合Debug视角，用于处理Debug情况下的视角，用曲线值表示使用什么样的数据
	TargetCameraRotation = UKismetMathLibrary::RLerp(InterpResult, DebugViewRotation,
													 GetAnimCurveValue(DebugOverrideCurve),
													 true);

	// 第三步，计算摄像机的枢轴位置，通过曲线反应的数据来混合具体的位置
	FVector LagSpeed(
		GetAnimCurveValue(LagPivotSpeedCurveX),
		GetAnimCurveValue(LagPivotSpeedCurveY),
		GetAnimCurveValue(LagPivotSpeedCurveZ)
		);

	// 对于摄像机的位置和目标位置，我们需要通过LagSpeed来计算一个差值(这里主要要转换到世界坐标下进行差值计算)
	const FVector& AxisIndpLag = CalculateLocationLagInCamera(SmoothedPivotTarget.GetLocation(),
															 PivotTarget.GetLocation(),
															 TargetCameraRotation,
															 LagSpeed,
															 DeltaTime);

	// 将平滑后的位置和旋转保存到变量中
	SmoothedPivotTarget.SetLocation(AxisIndpLag);
	SmoothedPivotTarget.SetRotation(PivotTarget.GetRotation());
	SmoothedPivotTarget.SetScale3D(FVector::OneVector);
	if(DrawDebugHint)
	{
		DrawDebugSphere(GetWorld(), AxisIndpLag, 10.0f, 36, FColor::Green, false, 0.1f, 0, 1.0f);
	}

	// 第四步，计算目标枢轴位置
	PivotLocation =
		SmoothedPivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(SmoothedPivotTarget.Rotator()) * GetAnimCurveValue(
			PivotOffsetCurveX) +
		UKismetMathLibrary::GetRightVector(SmoothedPivotTarget.Rotator()) * GetAnimCurveValue(
			PivotOffsetCurveY) +
		UKismetMathLibrary::GetUpVector(SmoothedPivotTarget.Rotator()) * GetAnimCurveValue(
			PivotOffsetCurveZ);

	if(DrawDebugHint)
	{
		DrawDebugSphere(GetWorld(), PivotLocation, 10.0f, 36, FColor::Red, false, 0.1f, 0, 1.0f);
	}
	// 第五步，计算摄像机的位置
	TargetCameraLocation = UKismetMathLibrary::VLerp(
		PivotLocation
		+UKismetMathLibrary::GetForwardVector(TargetCameraRotation) * GetAnimCurveValue(CameraOffsetCurveX)
		+UKismetMathLibrary::GetRightVector(TargetCameraRotation) * GetAnimCurveValue(CameraOffsetCurveY)
		+UKismetMathLibrary::GetUpVector(TargetCameraRotation) * GetAnimCurveValue(CameraOffsetCurveZ),
		PivotTarget.GetLocation() + DebugViewOffset,
		GetAnimCurveValue(DebugOverrideCurve));

	FVector PredictTPCameraLocation = PredictTPCameraTarget();
	if(DrawDebugHint)
	{
		DrawDebugSphere(GetWorld(), PredictTPCameraLocation, 10.0f, 36, FColor::Blue, false, 0.1f, 0, 1.0f);
	}
	
	Location = AxisIndpLag;
	Rotation = PivotTarget.GetRotation().Rotator();

	// 第六步，根据碰撞计算摄像机的偏移位置
	FVector TraceOrigin;
	float TraceRadius;
	TEnumAsByte<ECollisionChannel> TraceChannel;
	ControlledPawn->Execute_GetTPTraceParams(ControlledPawn, TraceOrigin, TraceRadius, TraceChannel);

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(ControlledPawn);

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
	float predictDistance = PreviousCameraLength;
	if (PredictHitResult.IsValidBlockingHit())
	{
		PredictTPCameraLocation += PredictHitResult.Location - PredictHitResult.TraceEnd;
		predictDistance = (PredictTPCameraLocation - TraceOrigin).Length();
	}
	if(predictHit && dis - predictDistance > GetAnimCurveValue(TraceIncreaseLagThresholdCurve))
	{
		// 插值调整 TargetCameraLocation，平滑移动以避免穿透
		FVector AdjustDirection = (TraceOrigin - TargetCameraLocation).GetSafeNormal(); // 计算调整方向
		FVector AdjustedLocation = TargetCameraLocation + AdjustDirection * (dis - predictDistance);
		FVector NowLocation = TargetCameraLocation + AdjustDirection * (dis - PreviousCameraLength);
		
		// 插值函数，平滑过渡到新的位置
		float alpha = DeltaTime * GetAnimCurveValue(TraceIncreaseLagSpeedCurve);
		TargetCameraLocation = FMath::Lerp(NowLocation, AdjustedLocation, alpha);
		
	}else if (dis - PreviousCameraLength > GetAnimCurveValue(TraceIncreaseLagThresholdCurve) || (InTraceLag && dis > PreviousCameraLength))
	{
		InTraceLag = true;
		float alpha = DeltaTime * GetAnimCurveValue(TraceIncreaseLagSpeedCurve);
		
		FVector Direction = (TraceOrigin - TargetCameraLocation).GetSafeNormal(); // 计算朝向

		FVector NewTargetLocation = TargetCameraLocation + Direction * (dis - PreviousCameraLength);
		TargetCameraLocation = FMath::Lerp(NewTargetLocation, TargetCameraLocation, alpha);
	}else
	{
		InTraceLag = false;
	}
	
	PreviousCameraLength = (TargetCameraLocation - TraceOrigin).Length();

	// 第七步，混合第一人称和第三人称的摄像机整体变换以及FOV
	FTransform TargetCameraTransform(TargetCameraRotation, TargetCameraLocation, FVector::OneVector);
	FTransform FPTargetCameraTransform(TargetCameraRotation, FPTarget, FVector::OneVector);

	const FTransform& MixedTransform = UKismetMathLibrary::TLerp(TargetCameraTransform, FPTargetCameraTransform,
																 GetAnimCurveValue(
																	 TargetFPWeightCurve));

	const FTransform& TargetTransform = UKismetMathLibrary::TLerp(MixedTransform,
																  FTransform(DebugViewRotation,
																  TargetCameraLocation,
																  FVector::OneVector),
																  GetAnimCurveValue(DebugOverrideCurve));

	Location = TargetTransform.GetLocation();
	Rotation = TargetTransform.Rotator();
	FOV = FMath::Lerp(TPFOV, FPFOV, GetAnimCurveValue(TargetFPWeightCurve));
	return true;
}