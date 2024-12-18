// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Camera/MeteorCameraInfoActor.h"


// Sets default values
AMeteorCameraInfoActor::AMeteorCameraInfoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeteorCameraInfoActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeteorCameraInfoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

