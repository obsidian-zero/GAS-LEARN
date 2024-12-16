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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	TObjectPtr<ADemoPlayerGASCharacterBase> ControlledPawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	TObjectPtr<USkeletalMeshComponent> CameraMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	TObjectPtr<UMeteorCameraBehavior> CameraBehavior;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Camera")
	void onPlayerControllerProcess(APawn* InPawn);
	virtual void onPlayerControllerProcess_Implementation(APawn* InPawn);

	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector PredictTPCameraTarget();
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector CalculateLocationLagInCamera(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);
	
	float GetAnimCurveValue(FName CurveName) const;

	// 曲线命名
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName LagRotatorSpeedCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName LagPivotSpeedCurveX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName LagPivotSpeedCurveY;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName LagPivotSpeedCurveZ;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName DebugOverrideCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName PivotOffsetCurveX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName PivotOffsetCurveY;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName PivotOffsetCurveZ;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName CameraOffsetCurveX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName CameraOffsetCurveY;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName CameraOffsetCurveZ;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName TargetFPWeightCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName TraceIncreaseLagThresholdCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Camera|Curve")
	FName TraceIncreaseLagSpeedCurve;
	
	// 曲线命名结束
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	FVector RootLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	FTransform SmoothedPivotTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	FVector PivotLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	FVector TargetCameraLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	FRotator TargetCameraRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	float PreviousCameraLength = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Info")
	bool InTraceLag = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Debug")
    FRotator DebugViewRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Debug")
	FVector DebugViewOffset;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Camera|Debug")
	float DrawDebugTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Camera|Debug")
	bool DrawDebugHint = false;
};
