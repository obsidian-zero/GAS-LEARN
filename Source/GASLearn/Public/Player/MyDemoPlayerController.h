// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyDemoPlayerController.generated.h"

class AMeteorPlayerCameraManager;

/**
 * 
 */
UCLASS()
class GASLEARN_API AMyDemoPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
