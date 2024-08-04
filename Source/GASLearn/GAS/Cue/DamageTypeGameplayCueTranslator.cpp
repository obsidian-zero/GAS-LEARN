// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/Cue/DamageTypeGameplayCueTranslator.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

void UDamageTypeGameplayCueTranslator::GetTranslationNameSpawns(TArray<FGameplayCueTranslationNameSwap>& SwapList) const
{
	{
		FGameplayCueTranslationNameSwap Temp;
		Temp.FromName = FName(TEXT("Normal"));
		Temp.ToNames.Add( FName(TEXT("Blaze")) );
		SwapList.Add(Temp);
	}

	{
		FGameplayCueTranslationNameSwap Temp;
		Temp.FromName = FName(TEXT("Damage.Normal"));
		Temp.ToNames.Add( FName(TEXT("Damage")) );
		Temp.ToNames.Add( FName(TEXT("Dark")) );
		SwapList.Add(Temp);
	}
	
}

int32 UDamageTypeGameplayCueTranslator::GameplayCueToTranslationIndex(const FName& TagName, AActor* TargetActor, const FGameplayCueParameters& Parameters) const
{
	if (!TargetActor)
	{
		return INDEX_NONE;
	}

	// 尝试获取 AbilitySystemComponent
	UAbilitySystemComponent* ASC = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		return INDEX_NONE;
	}

	// 创建一个 GameplayTag 进行匹配
	FGameplayTag BlazeTag = FGameplayTag::RequestGameplayTag(FName("TranCue.State.Blaze"));
	if (ASC->HasMatchingGameplayTag(BlazeTag))
	{
		return 0;
	}
	FGameplayTag DarkTag = FGameplayTag::RequestGameplayTag(FName("TranCue.State.Dark"));
	if (ASC->HasMatchingGameplayTag(DarkTag))
	{
		return 1;
	}
	
	return INDEX_NONE;
}

int32 UDamageTypeGameplayCueTranslator::GetPriority() const
{
	return 0;
}

bool UDamageTypeGameplayCueTranslator::IsEnabled() const
{
	return true;
}

bool UDamageTypeGameplayCueTranslator::ShouldShowInTopLevelFilterList() const
{
	return true;
}