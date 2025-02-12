// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"
#include "GameplayTags.h"
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "Components/ActorComponent.h"
#include "MeteorAbilityInputComponent.generated.h"


class UAbilitySystemComponent;
class UEnhancedInputComponent;
class UActionGameplayAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLEARN_API UMeteorAbilityInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeteorAbilityInputComponent();

	bool BindInputComponent(TObjectPtr<UEnhancedInputComponent> EIC);

	void TriggerInputAction(const FInputActionInstance& InputInstance);
	
	TArray<TObjectPtr<UActionGameplayAbility>> GetActionGameplayAbility(const UInputAction* Action);

	void BindActionInputAction(TObjectPtr<UInputAction> IA);

	void ActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions);

	void DeActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions);

	bool onAddActionGameplayAbility(TSubclassOf<UActionGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle);

	UFUNCTION()
	void onBeforeEvaluateInputDelegates();

	UFUNCTION()
	void onAfterEvaluateInputDelegates();

	void TriggerActionAbility();
	
	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
	
	TArray<TObjectPtr<UInputAction>> BindedInputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTag OnActionTag;

	TArray<const UInputAction *> TriggeredInputActions;
	
	TMap<TWeakObjectPtr<UInputAction>, TObjectPtr<UActionGameplayAbility>> InputActionToBasicAbilityMap;
	TMap<TWeakObjectPtr<UInputAction>, TObjectPtr<UActionGameplayAbility>> InputActionToComboAbilityMap;
	TMap<TWeakObjectPtr<UInputAction>, TObjectPtr<UActionGameplayAbility>> InputActionToAlwaysAbilityMap;
	TMap<TObjectPtr<UActionGameplayAbility>, TArray<TObjectPtr<UInputAction>>> ActionAbilityToInputActionMap;
	TMap<TObjectPtr<UActionGameplayAbility>, FGameplayAbilitySpecHandle> ActionAbilityToSpec;
	
	TWeakObjectPtr<UEnhancedInputComponent> BindedEnhancedInputComponent;
	
};
