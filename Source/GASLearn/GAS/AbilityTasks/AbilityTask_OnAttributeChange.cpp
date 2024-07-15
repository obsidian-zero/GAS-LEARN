// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/AbilityTasks/AbilityTask_OnAttributeChange.h"

#include "AbilitySystemComponent.h"

UAbilityTask_OnAttributeChange::UAbilityTask_OnAttributeChange(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TriggerOnce = false;
}

void UAbilityTask_OnAttributeChange::Activate()
{
	if(AbilitySystemComponent.Get() && Attribute.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UAbilityTask_OnAttributeChange::OnAttributeChanged);
	}
	
	Super::Activate();
}

UAbilityTask_OnAttributeChange* UAbilityTask_OnAttributeChange::ListenForAttributeChange(
	UGameplayAbility* OwningAbility,
	bool TriggerOnce,
	UAbilitySystemComponent* SystemComponent,
	FGameplayAttribute Attribute)
{
	UAbilityTask_OnAttributeChange* MyObj = NewAbilityTask<UAbilityTask_OnAttributeChange>(OwningAbility);
	MyObj->TriggerOnce = TriggerOnce;
	MyObj->AbilitySystemComponent = SystemComponent;
	MyObj->Attribute = Attribute;

	return MyObj;
}

void UAbilityTask_OnAttributeChange::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	AttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);

	if(TriggerOnce)
	{
		EndTask();
	}
}

void UAbilityTask_OnAttributeChange::OnDestroy(bool bInOwnerFinished)
{
	if(AbilitySystemComponent.Get())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
	}
	Super::OnDestroy(bInOwnerFinished);
}