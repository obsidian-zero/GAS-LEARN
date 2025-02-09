// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "MeteorInputTriggerCombo.generated.h"

class UEnhancedPlayerInput;
class UInputAction;
/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "MeteorCombo", NotInputConfigurable = "true"))
class GASLEARN_API UMeteorInputTriggerCombo : public UInputTrigger
{
	GENERATED_BODY()

	UMeteorInputTriggerCombo();

protected:
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	float TotalTimeForUse = 2.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Trigger Settings")
	float CurrentTimeIncomboTotal = 0.0f;

	int32 SameBeginIANum = 1;
	TArray<float> TimeForComboSteps;
	TObjectPtr<const UInputAction> BeginInputAction;
	
	// Implicit, so action cannot fire unless this is firing.
	virtual ETriggerType GetTriggerType_Implementation() const override { return ETriggerType::Implicit; }

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	// Keeps track of what action we're currently at in the combo
	UPROPERTY(BlueprintReadOnly, Category = "Trigger Settings")
	int32 CurrentComboStepIndex = 0;
	
	// Time elapsed between last combo InputAction trigger and current time
	UPROPERTY(BlueprintReadOnly, Category = "Trigger Settings")
	float CurrentTimeBetweenComboSteps = 0.0f;

public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

#if  WITH_EDITORONLY_DATA
	// Actions that will cancel the combo if they are triggered.
	UE_DEPRECATED(5.2, "CancelActions has been deprecated as of 5.2. Please use InputCancelActions instead.")
	TArray<TObjectPtr<const UInputAction>> CancelActions;
#endif

	virtual void PostLoad() override;
	
	/**
	 * List of input actions that need to be completed (according to Combo Step Completion States) to trigger this action.
	 * Input actions must be triggered in order (starting at index 0) to count towards the triggering of the combo.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings", meta = (DisplayThumbnail = "false", TitleProperty = "ComboStepAction"))
	TArray<FInputComboStepData> ComboActions;

	// Actions that will cancel the combo if they are completed (according to Cancellation States)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings", meta = (DisplayThumbnail = "false", DisplayName = "Cancel Actions"))
	TArray<FInputCancelAction> InputCancelActions;

	/** Determines what kind of trigger events can happen from the behavior of this trigger. */
	virtual ETriggerEventsSupported GetSupportedTriggerEvents() const override { return ETriggerEventsSupported::All; }
};