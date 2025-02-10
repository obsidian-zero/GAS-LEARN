// File: Source/GASLearn/EnhancedInput/MeteorAbilityInputComponent.cpp

#include "MeteorAbilityInputComponent.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "GASLearn/GAS/Ability/ActionGameplayAbility.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "Engine/Engine.h"

UMeteorAbilityInputComponent::UMeteorAbilityInputComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    
    OnActionTag = FGameplayTag::RequestGameplayTag(FName("SpecialTag.ActionState.OnAction"));
}

bool UMeteorAbilityInputComponent::BindInputComponent(TObjectPtr<UEnhancedInputComponent> EIC)
{
    if (EIC)
    {
        BindedEnhancedInputComponent = EIC;

        for (TPair<TSubclassOf<UActionGameplayAbility>, TArray<TObjectPtr<UInputAction>>>& Pair : ActionAbilityToInputActionMap)
        {
            for(TObjectPtr<UInputAction> IA: Pair.Value)
            {
                BindActionInputAction(IA);
            }
        }
        return true;
    }

    return false;
}

void UMeteorAbilityInputComponent::UseActionGameplayAbility(const FInputActionInstance& InputInstance)
{
    const UInputAction* Action = InputInstance.GetSourceAction();
    if (AbilitySystemComponent.IsValid())
    {
        if (AbilitySystemComponent->HasMatchingGameplayTag(OnActionTag))
        {
            FGameplayAbilitySpecHandle* comboSpec = InputActionToComboAbilityMap.Find(Action);
            if (comboSpec)
            {
                AbilitySystemComponent->TryActivateAbility(*comboSpec, true);
            }
        }
        else
        {
            FGameplayAbilitySpecHandle* basicSpec = InputActionToBasicAbilityMap.Find(Action);
            if (basicSpec)
            {
                AbilitySystemComponent->TryActivateAbility(*basicSpec, true);
            }
            else
            {
                UE_LOG(LogInput, Warning, TEXT("InputActionToAbilityMap does not contain %s"), *Action->GetFName().ToString());
            }
        }

        if (InputActionToAlwaysAbilityMap.Contains(Action))
        {
            FGameplayAbilitySpecHandle* alwaysSpec = InputActionToAlwaysAbilityMap.Find(Action);
            if (alwaysSpec)
            {
                AbilitySystemComponent->TryActivateAbility(*alwaysSpec, true);
            }
        }
    }
    else
    {
        UE_LOG(LogInput, Warning, TEXT("AbilitySystemComponent is not valid"));
    }
}

void UMeteorAbilityInputComponent::BindActionInputAction(TObjectPtr<UInputAction> IA)
{
    if (!BindedInputActions.Contains(IA) && BindedEnhancedInputComponent.Get())
    {
        BindedEnhancedInputComponent->BindAction(IA, ETriggerEvent::Completed, this, &UMeteorAbilityInputComponent::UseActionGameplayAbility);
        BindedInputActions.Add(IA);
    }
}

void UMeteorAbilityInputComponent::ActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions)
{
    if (ActionAbilityToSpec.Contains(Ability))
    {
        FGameplayAbilitySpecHandle AbilitySpecHandle = ActionAbilityToSpec[Ability];

        for (UInputAction* InputAction : InputActions)
        {
            if (!BindedInputActions.Contains(InputAction))
            {
                BindActionInputAction(InputAction);
            }

            if (!InputActionToComboAbilityMap.Find(InputAction))
            {
                InputActionToComboAbilityMap.Add(InputAction, AbilitySpecHandle);
            }
            else
            {
                UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s"), *InputAction->GetName(), *Ability->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogInput, Warning, TEXT("ActionAbilityToSpec does not contain ability %s"), *Ability->GetName());
    }
}

void UMeteorAbilityInputComponent::DeActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions)
{
    if (ActionAbilityToSpec.Contains(Ability))
    {
        FGameplayAbilitySpecHandle AbilitySpecHandle = ActionAbilityToSpec[Ability];

        for (UInputAction* InputAction : InputActions)
        {
            if (InputActionToComboAbilityMap.Find(InputAction))
            {
                if (InputActionToComboAbilityMap[InputAction] != AbilitySpecHandle)
                {
                    UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s"), *InputAction->GetName(), *Ability->GetName());
                }
                else
                {
                    InputActionToComboAbilityMap.Remove(InputAction);
                }
            }
            else
            {
                UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap not contains action %s, ability: %s"), *InputAction->GetName(), *Ability->GetName());
            }
        }
    }
    else
    {
        UE_LOG(LogInput, Warning, TEXT("ActionAbilityToSpec does not contain ability %s"), *Ability->GetName());
    }
}

bool UMeteorAbilityInputComponent::onAddActionGameplayAbility(TSubclassOf<UActionGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle)
{
    if (Ability && GetOwnerRole() == ROLE_Authority && AbilitySystemComponent.IsValid())
    {
        ActionAbilityToSpec.Add(Ability, AbilitySpecHandle);

        TArray<TObjectPtr<UInputAction>> AbilityInputActions;

        for (UInputAction* IA : Ability->GetDefaultObject<UActionGameplayAbility>()->BasicInputActions)
        {
            BindActionInputAction(IA);

            if (!InputActionToBasicAbilityMap.Contains(IA))
            {
                InputActionToBasicAbilityMap.Add(IA, AbilitySpecHandle);
            }
            else
            {
                FString basicName = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability.GetName();
                UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s when add ability: %s"), *IA->GetName(), *basicName, *Ability->GetName());
            }
            AbilityInputActions.Add(IA);
        }

        for (UInputAction* IA : Ability->GetDefaultObject<UActionGameplayAbility>()->ComboInputActions)
        {
            BindActionInputAction(IA);
            AbilityInputActions.Add(IA);
        }

        for (UInputAction* IA : Ability->GetDefaultObject<UActionGameplayAbility>()->AlwaysInputActions)
        {
            BindActionInputAction(IA);

            if (!InputActionToAlwaysAbilityMap.Contains(IA))
            {
                InputActionToAlwaysAbilityMap.Add(IA, AbilitySpecHandle);
            }
            else
            {
                FString basicName = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability.GetName();
                UE_LOG(LogInput, Warning, TEXT("InputActionToAlwaysAbilityMap already contains action %s, ability: %s when add ability: %s"), *IA->GetName(), *basicName, *Ability->GetName());
            }
            AbilityInputActions.Add(IA);
        }

        ActionAbilityToInputActionMap.Add(Ability, AbilityInputActions);

        return true;
    }

    return false;
}