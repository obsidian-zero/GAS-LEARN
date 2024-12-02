// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/MeteorPlayerCameraManager.h"
#include "GASLearn/Camera/MeteorCameraInterface.h"
#include "GASLearn/Public/Character/Player/DemoPlayerGASCharacterBase.h"
#include "GASLearn/Camera/MeteorCameraBehavior.h"
#include "Kismet/KismetMathLibrary.h"

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

FVector AMeteorPlayerCameraManager::CalculateLocationLagInWorld(FVector CurrentLocation, FVector TargetLocation,
															 FRotator CameraRotation, FVector LagSpeeds,
															 float DeltaTime)
{
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;
	const FVector UnrotatedCurLoc = CameraRotation.UnrotateVector(CurrentLocation);
	const FVector UnrotatedTargetLoc = CameraRotation.UnrotateVector(TargetLocation);

	const FVector ResultVector(
		FMath::FInterpTo(UnrotatedCurLoc.X, UnrotatedTargetLoc.X, DeltaTime, LagSpeeds.X),
		FMath::FInterpTo(UnrotatedCurLoc.Y, UnrotatedTargetLoc.Y, DeltaTime, LagSpeeds.Y),
		FMath::FInterpTo(UnrotatedCurLoc.Z, UnrotatedTargetLoc.Z, DeltaTime, LagSpeeds.Z));

	return CameraRotation.RotateVector(ResultVector);
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

	// 第三步，计算摄像机的位置，通过曲线反应的数据来混合具体的位置
	FVector LagSpeed(
		GetAnimCurveValue(LagPivotSpeedCurveX),
		GetAnimCurveValue(LagPivotSpeedCurveY),
		GetAnimCurveValue(LagPivotSpeedCurveZ)
		);

	// 对于摄像机的位置和目标位置，我们需要通过LagSpeed来计算一个差值(这里主要要转换到世界坐标下进行差值计算)
	const FVector& AxisIndpLag = CalculateLocationLagInWorld(SmoothedPivotTarget.GetLocation(),
															 PivotTarget.GetLocation() - FVector(100.0f, 0.0f, 0.0f),
															 TargetCameraRotation,
															 LagSpeed,
															 DeltaTime);

	SmoothedPivotTarget.SetLocation(AxisIndpLag);
	SmoothedPivotTarget.SetRotation(PivotTarget.GetRotation());
	SmoothedPivotTarget.SetScale3D(FVector::OneVector);

	Location = AxisIndpLag;
	Rotation = PivotTarget.GetRotation().Rotator();
	
	return true;
}