// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoAIController.h"
#include "AbilitySystemComponent.h"
#include "Player/MyPlayerState.h"

void ADemoAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
		
	}
}

