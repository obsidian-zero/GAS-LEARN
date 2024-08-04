// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/Cue/DefenceGameplayCueTranslator.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

void UDefenceGameplayCueTranslator::GetTranslationNameSpawns(TArray<FGameplayCueTranslationNameSwap>& SwapList) const
{
	{
		FGameplayCueTranslationNameSwap Temp;
		Temp.FromName = FName(TEXT("Normal"));
		Temp.ToNames.Add( FName(TEXT("Blaze")) );
		SwapList.Add(Temp);
	}

	{
		FGameplayCueTranslationNameSwap Temp;
		Temp.FromName = FName(TEXT("Defence.Normal"));
		Temp.ToNames.Add( FName(TEXT("Defence")) );
		Temp.ToNames.Add( FName(TEXT("Dark")) );
		SwapList.Add(Temp);
	}
	
}

int32 UDefenceGameplayCueTranslator::GameplayCueToTranslationIndex(const FName& TagName, AActor* TargetActor, const FGameplayCueParameters& Parameters) const
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
