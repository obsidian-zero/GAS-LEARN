// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/Ability/MeteorGameplayAbility.h"

void UMeteorGameplayAbility::CollectAllGameplayEffects(UObject* GameplayAbility, TArray<TSubclassOf<UGameplayEffect>>& OutEffects)
{
	if (UBlueprint* Blueprint = Cast<UBlueprint>(GameplayAbility->GetClass()->ClassGeneratedBy))
	{
		const TArray<UEdGraph*> Graphs = Blueprint->UbergraphPages;
		for (UEdGraph* Graph : Graphs)
		{
			for (UEdGraphNode* Node : Graph->Nodes)
			{
				if (UK2Node_CallFunction* CallFunctionNode = Cast<UK2Node_CallFunction>(Node))
				{
					FName FunctionName = CallFunctionNode->GetFunctionName();

					// 检查是否是与 GameplayEffect 相关的函数
					if (FunctionName == FName("BP_ApplyGameplayEffectToTarget") || 
						FunctionName == FName("BP_ApplyGameplayEffectToOwner"))
					{
						if (UEdGraphPin* Pin = CallFunctionNode->FindPin(TEXT("GameplayEffectClass")))
						{
							if (Pin->DefaultObject)
							{
								if (UClass* EffectClass = Cast<UClass>(Pin->DefaultObject))
								{
									if (EffectClass->IsChildOf(UGameplayEffect::StaticClass()))
									{
										OutEffects.AddUnique(EffectClass);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void UMeteorGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TArray<TSubclassOf<UGameplayEffect>> Effects;
	CollectAllGameplayEffects(this, Effects);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}