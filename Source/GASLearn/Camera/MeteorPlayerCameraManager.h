// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MeteorPlayerCameraManager.generated.h"

class ADemoPlayerGASCharacterBase;
class UMeteorCameraBehavior;
/**
 * 
 */
UCLASS()
class GASLEARN_API AMeteorPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	AMeteorPlayerCameraManager();

public:
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
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static FVector CalculateLocationLagInWorld(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);
	
	float GetAnimCurveValue(FName CurveName) const;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Debug")
    FRotator DebugViewRotation;
};
