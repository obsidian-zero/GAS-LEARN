// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyDemoPlayerController.h"
#include "AbilitySystemComponent.h"
#include "Player/MyPlayerState.h"
#include "GASLearn/Camera/MeteorPlayerCameraManager.h"

void AMyDemoPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
	if(AMeteorPlayerCameraManager * CameraManager = Cast<AMeteorPlayerCameraManager>(PlayerCameraManager))
	{
		CameraManager->onPlayerControllerProcess(InPawn);
	}
}
