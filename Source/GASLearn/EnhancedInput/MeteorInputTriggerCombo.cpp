// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/EnhancedInput/MeteorInputTriggerCombo.h"
#include "EnhancedInputModule.h"
#include "EnhancedPlayerInput.h"

ETriggerState UMeteorInputTriggerCombo::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	if (ComboActions.IsEmpty())
	{
		UE_LOG(LogEnhancedInput, Warning, TEXT("A Combo Trigger has no combo actions and will not work properly!"));
		return ETriggerState::None;
	}
	
	if (const UInputAction* CurrentAction = ComboActions[CurrentComboStepIndex].ComboStepAction)
	{
		// loop through all cancel actions and check if they've fired
		for (FInputCancelAction InputCancelActionData : InputCancelActions)
		{
			const UInputAction* CancelAction = InputCancelActionData.CancelAction;
			if (CancelAction && CancelAction != CurrentAction)
			{
				const FInputActionInstance* CancelState = PlayerInput->FindActionInstanceData(CancelAction);
				// Check the cancellation state against the states that should cancel the combo
				if (CancelState && (InputCancelActionData.CancellationStates & static_cast<uint8>(CancelState->GetTriggerEvent())))
				{
					// Cancel action firing!
					CurrentComboStepIndex = 0;
					CurrentTimeIncomboTotal = 0;
					CurrentAction = ComboActions[CurrentComboStepIndex].ComboStepAction;	// Reset for fallthrough
					break;
				}
			}
		}
		// loop through all combo actions and check if a combo action fired out of order
		for (FInputComboStepData ComboStep : ComboActions)
		{
			if (ComboStep.ComboStepAction && ComboStep.ComboStepAction != CurrentAction)
			{
				const FInputActionInstance* CancelState = PlayerInput->FindActionInstanceData(ComboStep.ComboStepAction);
				// Check the combo action state against the states that should complete this step
				if (CancelState && (ComboStep.ComboStepCompletionStates & static_cast<uint8>(CancelState->GetTriggerEvent())))
				{
					// Other combo action firing - should cancel
					CurrentComboStepIndex = 0;
					CurrentTimeIncomboTotal = 0;
					CurrentAction = ComboActions[CurrentComboStepIndex].ComboStepAction;	// Reset for fallthrough
					break;
				}
			}
		}

		// Reset if we take too long to hit the action
		if (CurrentComboStepIndex > 0)
		{
			CurrentTimeBetweenComboSteps += DeltaTime;
			CurrentTimeIncomboTotal += DeltaTime;
			if (CurrentTimeBetweenComboSteps >= ComboActions[CurrentComboStepIndex].TimeToPressKey || CurrentTimeIncomboTotal >= TotalTimeForUse)
			{
				CurrentComboStepIndex = 0;
				CurrentTimeIncomboTotal = 0;
				CurrentAction = ComboActions[CurrentComboStepIndex].ComboStepAction;	// Reset for fallthrough			
			}
		}

		const FInputActionInstance* CurrentState = PlayerInput->FindActionInstanceData(CurrentAction);
		// check to see if current action is in one of it's completion states - if so advance the combo to the next combo action
		if (CurrentState && (ComboActions[CurrentComboStepIndex].ComboStepCompletionStates & static_cast<uint8>(CurrentState->GetTriggerEvent())))
		{
			CurrentComboStepIndex++;
			CurrentTimeBetweenComboSteps = 0;
			// check to see if we've completed all actions in the combo
			if (CurrentComboStepIndex >= ComboActions.Num())
			{
				CurrentComboStepIndex = 0;
				CurrentTimeIncomboTotal = 0;
				return ETriggerState::Triggered;
			}
		}

		if (CurrentComboStepIndex > 0)
		{
			return ETriggerState::Ongoing;
		}
	
		// Really should account for first combo action being mid-trigger...
		const FInputActionInstance* InitialState = PlayerInput->FindActionInstanceData(ComboActions[0].ComboStepAction);
		if (InitialState && InitialState->GetTriggerEvent() > ETriggerEvent::None) // || Cancelled!
		{
			return ETriggerState::Ongoing;
		}
		CurrentTimeBetweenComboSteps = 0;
	}
	return ETriggerState::None;
};