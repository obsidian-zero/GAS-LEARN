// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MeteorCameraBehavior.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorCameraBehavior : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Metero|Camera")
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Metero|Camera")
	APawn* PlayerPawn;
};
