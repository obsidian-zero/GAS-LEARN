// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GASLearn/Camera/MeteorCameraInfoBase.h"
#include "MeteorCameraInfoActor.generated.h"

UCLASS()
class GASLEARN_API AMeteorCameraInfoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteorCameraInfoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "CameraSystem")
	TObjectPtr<UMeteorCameraInfoBase> CameraInfo;
	
};
