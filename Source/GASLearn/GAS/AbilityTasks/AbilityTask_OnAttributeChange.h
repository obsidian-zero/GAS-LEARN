// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_OnAttributeChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

/**
 * 
 */
UCLASS()
class GASLEARN_API UAbilityTask_OnAttributeChange : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_OnAttributeChange(const FObjectInitializer& ObjectInitializer);
	
	virtual void Activate() override;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeChanged AttributeChanged;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="TRUE"))
	static UAbilityTask_OnAttributeChange* ListenForAttributeChange(
		UGameplayAbility* OwningAbility,
		bool TriggerOnce,
		UAbilitySystemComponent* SystemComponent,
		FGameplayAttribute Attribute);

protected:
	bool TriggerOnce;

	FGameplayAttribute Attribute;

	void OnAttributeChanged(const FOnAttributeChangeData& Data);

	virtual void OnDestroy(bool bInOwnerFinished) override;
	
};