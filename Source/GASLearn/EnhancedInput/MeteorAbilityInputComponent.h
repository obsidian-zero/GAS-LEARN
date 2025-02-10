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
	
	void UseActionGameplayAbility(const FInputActionInstance& InputInstance);

	void BindActionInputAction(TObjectPtr<UInputAction> IA);

	void ActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions);

	void DeActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions);

	bool onAddActionGameplayAbility(TSubclassOf<UActionGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle);

	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
	
	TArray<TObjectPtr<UInputAction>> BindedInputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTag OnActionTag;
	
	TMap<UInputAction*, FGameplayAbilitySpecHandle> InputActionToBasicAbilityMap;
	TMap<UInputAction*, FGameplayAbilitySpecHandle> InputActionToComboAbilityMap;
	TMap<UInputAction*, FGameplayAbilitySpecHandle> InputActionToAlwaysAbilityMap;
	TMap<TSubclassOf<UActionGameplayAbility>, TArray<TObjectPtr<UInputAction>>> ActionAbilityToInputActionMap;
	TMap<TSubclassOf<UActionGameplayAbility>, FGameplayAbilitySpecHandle> ActionAbilityToSpec;

	TWeakObjectPtr<UEnhancedInputComponent> BindedEnhancedInputComponent;
	
};
