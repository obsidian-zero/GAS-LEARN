// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/Attributes/MonsterAttributeSetBase.h"

bool UMonsterAttributeSetBase::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("UMonsterAttributeSetBase::PreGameplayEffectExecute"));
	return true;
}