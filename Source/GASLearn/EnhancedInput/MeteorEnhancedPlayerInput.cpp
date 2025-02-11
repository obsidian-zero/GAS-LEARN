// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/EnhancedInput/MeteorEnhancedPlayerInput.h"

void UMeteorEnhancedPlayerInput::EvaluateInputDelegates(const TArray<UInputComponent*>& InputComponentStack, float DeltaTime, bool bGamePaused, const TArray<TPair<FKey, FKeyState*>>& KeysWithEvents)
{
	// Call the before delegate
	OnBeforeEvaluateInputDelegates.Broadcast();

	// Call the base class implementation
	Super::EvaluateInputDelegates(InputComponentStack, DeltaTime, bGamePaused, KeysWithEvents);

	// Call the after delegate
	OnAfterEvaluateInputDelegates.Broadcast();
}