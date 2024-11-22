// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/EnhancedInput/MeteorInputTriggerCombo.h"
#include "EnhancedInputModule.h"
#include "EnhancedPlayerInput.h"
#include "InputTriggers.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "EnhancedInputTriggers"

UMeteorInputTriggerCombo::UMeteorInputTriggerCombo()
{
	bShouldAlwaysTick = true;
}

#if WITH_EDITOR
EDataValidationResult UMeteorInputTriggerCombo::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
	
	// You can't evaluate the combo if there are no combo steps!
	if (ComboActions.IsEmpty())
	{
		Result = EDataValidationResult::Invalid;
		Context.AddError(LOCTEXT("NoComboSteps", "There must be at least one combo step in the Combo Trigger!"));
	}

	// Making sure combo completion states have at least one state
	for (const FInputComboStepData& ComboStep : ComboActions)
	{
		if (ComboStep.ComboStepCompletionStates == 0)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("NoCompletionStates", "There must be at least one completion state in ComboStep Completion States in the {0} combo step in order to progress the combo!"), FText::FromString(ComboStep.ComboStepAction.GetName())));
		}
	}

	// Making sure cancellation states have at least one state
	for (const FInputCancelAction& CancelAction : InputCancelActions)
	{
		if (CancelAction.CancellationStates == 0)
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("NoCancellationStates", "There must be at least one cancellation state in Cancellation States in the {0} cancel action in order to cancel the combo!"), FText::FromString(CancelAction.CancelAction.GetName())));
		}
	}

	return Result;
}
#endif // WITH_EDITOR

void UMeteorInputTriggerCombo::PostLoad()
{
	Super::PostLoad();
	
	// start fix up in case it's an old version that just has the cancel input action
#if WITH_EDITORONLY_DATA
	PRAGMA_DISABLE_DEPRECATION_WARNINGS

	if (!CancelActions.IsEmpty())
	{
		for (const TObjectPtr<const UInputAction>& InputAction : CancelActions)
		{
			// use default settings but set cancel action
			FInputCancelAction InputCancelAction;
			InputCancelAction.CancelAction = InputAction;
			InputCancelActions.Add(InputCancelAction);
		}
		CancelActions.Empty();
	}
		
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
#endif // WITH_EDITORONLY_DATA
}


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

#undef LOCTEXT_NAMESPACE