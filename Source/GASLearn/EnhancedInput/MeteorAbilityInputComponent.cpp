// File: Source/GASLearn/EnhancedInput/MeteorAbilityInputComponent.cpp

#include "MeteorAbilityInputComponent.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "GASLearn/GAS/Ability/ActionGameplayAbility.h"
#include "GASLearn/EnhancedInput/MeteorEnhancedPlayerInput.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "Engine/Engine.h"

UMeteorAbilityInputComponent::UMeteorAbilityInputComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    
    OnActionTag = FGameplayTag::RequestGameplayTag(FName("SpecialTag.ActionState.OnAction"));

    inputLock = 0;
}

bool UMeteorAbilityInputComponent::BindInputComponent(TObjectPtr<UEnhancedInputComponent> EIC)
{
    if (EIC)
    {
        BindedEnhancedInputComponent = EIC;

        for (TPair<TObjectPtr<UActionGameplayAbility>, TArray<TObjectPtr<UInputAction>>>& Pair : ActionAbilityToInputActionMap)
        {
            for(TObjectPtr<UInputAction> IA: Pair.Value)
            {
                BindActionInputAction(IA);
            }
        }

        if(TObjectPtr<ACharacter> Owner = Cast<ACharacter>(GetOwner()))
        {
            if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Owner->Controller))
            {
                if(TObjectPtr<UMeteorEnhancedPlayerInput> EnhancedPlayerInput = Cast<UMeteorEnhancedPlayerInput>(PlayerController->PlayerInput))
                {
                    EnhancedPlayerInput->OnBeforeEvaluateInputDelegates.AddDynamic(this, &UMeteorAbilityInputComponent::onBeforeEvaluateInputDelegates);
                    EnhancedPlayerInput->OnAfterEvaluateInputDelegates.AddDynamic(this, &UMeteorAbilityInputComponent::onAfterEvaluateInputDelegates);
                }
            }
        }
        return true;
    }

    return false;
}

void UMeteorAbilityInputComponent::TriggerInputAction(const FInputActionInstance& InputInstance)
{
    const UInputAction* IA = InputInstance.GetSourceAction();
    if(IsInputBufferLocked())
    {
        // If the input buffer is locked, we will only add the allowed input actions to the buffer
        if(AllowBufferInputActions.FindOrAdd(IA, 0) == 0)
        {
            return;
        }
    }
    
    if(!TriggeredInputActions.Contains(IA))
    {
        TriggeredInputActions.Add(IA);
    }
}


TArray<TObjectPtr<UActionGameplayAbility>> UMeteorAbilityInputComponent::GetActionGameplayAbility(const UInputAction* Action)
{
    TArray<TObjectPtr<UActionGameplayAbility>> Abilities;
    if (AbilitySystemComponent.IsValid())
    {
        TObjectPtr<UActionGameplayAbility> * Ability = nullptr;
        if (AbilitySystemComponent->HasMatchingGameplayTag(OnActionTag))
        {
            Ability = InputActionToComboAbilityMap.Find(Action);
            if (Ability)
            {
                Abilities.Add(*Ability);
            }
            else
            {
                UE_LOG(LogInput, Warning, TEXT("InputActionToAbilityMap does not contain %s"), *Action->GetFName().ToString());
            }
        }
        else
        {
            Ability = InputActionToBasicAbilityMap.Find(Action);
            if (Ability)
            {
                Abilities.Add(*Ability);
            }
            else
            {
                UE_LOG(LogInput, Warning, TEXT("InputActionToAbilityMap does not contain %s"), *Action->GetFName().ToString());
            }
        }

        Ability = nullptr;
        
        if (InputActionToAlwaysAbilityMap.Contains(Action))
        {
            Ability = InputActionToAlwaysAbilityMap.Find(Action);
            if (Ability)
            {
                Abilities.Add(*Ability);
            }
        }
    }
    else
    {
        UE_LOG(LogInput, Warning, TEXT("AbilitySystemComponent is not valid"));
    }
    return Abilities;
}

void UMeteorAbilityInputComponent::BindActionInputAction(TObjectPtr<UInputAction> IA)
{
    if (!BindedInputActions.Contains(IA) && BindedEnhancedInputComponent.Get())
    {
        BindedEnhancedInputComponent->BindAction(IA, ETriggerEvent::Completed, this, &UMeteorAbilityInputComponent::TriggerInputAction);
        BindedInputActions.Add(IA);
    }
}

void UMeteorAbilityInputComponent::ActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions)
{
    TObjectPtr<UActionGameplayAbility> AbilityObj = Ability.GetDefaultObject();
    if (ActionAbilityToSpec.Contains(AbilityObj))
    {
        FGameplayAbilitySpecHandle AbilitySpecHandle = ActionAbilityToSpec[AbilityObj];

        for (UInputAction* InputAction : InputActions)
        {
            if (!BindedInputActions.Contains(InputAction))
            {
                BindActionInputAction(InputAction);
            }

            if (!InputActionToComboAbilityMap.Find(InputAction))
            {
                InputActionToComboAbilityMap.Add(InputAction, AbilityObj);
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
    TObjectPtr<UActionGameplayAbility> AbilityObj = Ability.GetDefaultObject();
    if (ActionAbilityToSpec.Contains(AbilityObj))
    {
        FGameplayAbilitySpecHandle AbilitySpecHandle = ActionAbilityToSpec[AbilityObj];

        for (UInputAction* InputAction : InputActions)
        {
            if (InputActionToComboAbilityMap.Find(InputAction))
            {
                if (InputActionToComboAbilityMap[InputAction] != AbilityObj)
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
        TObjectPtr<UActionGameplayAbility> AbilityObj = Ability.GetDefaultObject();
        
        ActionAbilityToSpec.Add(AbilityObj, AbilitySpecHandle);

        TArray<TObjectPtr<UInputAction>> AbilityInputActions;
        
        for (UInputAction* IA : AbilityObj->BasicInputActions)
        {
            BindActionInputAction(IA);

            if (!InputActionToBasicAbilityMap.Contains(IA))
            {
                InputActionToBasicAbilityMap.Add(IA, AbilityObj);
            }
            else
            {
                FString basicName = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability.GetName();
                UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s when add ability: %s"), *IA->GetName(), *basicName, *Ability->GetName());
            }
            AbilityInputActions.Add(IA);
        }

        for (UInputAction* IA : AbilityObj->ComboInputActions)
        {
            BindActionInputAction(IA);
            AbilityInputActions.Add(IA);
        }

        for (UInputAction* IA : AbilityObj->AlwaysInputActions)
        {
            BindActionInputAction(IA);

            if (!InputActionToAlwaysAbilityMap.Contains(IA))
            {
                InputActionToAlwaysAbilityMap.Add(IA, AbilityObj);
            }
            else
            {
                FString basicName = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability.GetName();
                UE_LOG(LogInput, Warning, TEXT("InputActionToAlwaysAbilityMap already contains action %s, ability: %s when add ability: %s"), *IA->GetName(), *basicName, *Ability->GetName());
            }
            AbilityInputActions.Add(IA);
        }

        ActionAbilityToInputActionMap.Add(AbilityObj, AbilityInputActions);

        return true;
    }

    return false;
}

void UMeteorAbilityInputComponent::StartInputBufferLock(TArray<UInputAction*> InputActions)
{
    inputLock += 1;
    for(UInputAction* IA: InputActions)
    {
        AllowBufferInputActions[IA] = AllowBufferInputActions.FindOrAdd(IA, 0) + 1;
    }
}

void UMeteorAbilityInputComponent::EndInputBufferLock(TArray<UInputAction*> InputActions)
{
    inputLock -= 1;
    for(UInputAction* IA: InputActions)
    {
        AllowBufferInputActions[IA] = AllowBufferInputActions.FindOrAdd(IA, 1) - 1;
    }
}

bool UMeteorAbilityInputComponent::IsInputBufferLocked()
{
    return inputLock > 0;
}


void UMeteorAbilityInputComponent::onBeforeEvaluateInputDelegates()
{
    if(!IsInputBufferLocked())
    {
        // This is the function that will be called when the input buffer unlock
        TriggerActionAbility(); 
    }
}

void UMeteorAbilityInputComponent::onAfterEvaluateInputDelegates()
{
    if(!IsInputBufferLocked())
    {
        TriggerActionAbility();
    }
}

void UMeteorAbilityInputComponent::TriggerActionAbility()
{
    TArray<TObjectPtr<UActionGameplayAbility>> Abilities;
    
    for(const UInputAction* IA: TriggeredInputActions)
    {
        TArray<TObjectPtr<UActionGameplayAbility>> TriggeredAbilities = GetActionGameplayAbility(IA);
        for(TObjectPtr<UActionGameplayAbility> Ability: TriggeredAbilities)
        {
            if(!Abilities.Contains(Ability))
            {
                Abilities.Add(Ability);
            }
        }
    }

    Abilities.StableSort([](const TObjectPtr<UActionGameplayAbility>& A, const TObjectPtr<UActionGameplayAbility>& B)
    {
        return A->GetInputPriority() > B->GetInputPriority();
    });
    
    if(AbilitySystemComponent.Get() && Abilities.Num() > 0)
    {
        AbilitySystemComponent->TryActivateAbility(ActionAbilityToSpec[Abilities[0]]);
    }
    TriggeredInputActions.Empty();
    AllowBufferInputActions.Empty();
}