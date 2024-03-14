// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSetBase.h"

#include "Net/UnrealNetwork.h"

void UCharacterAttributeSetBase::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Level, OldLevel);
}

void UCharacterAttributeSetBase::OnRep_HandSize(const FGameplayAttributeData& OldHandSize)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, HandSize, OldHandSize);
}

void UCharacterAttributeSetBase::OnRep_MaxHandSize(const FGameplayAttributeData& OldMaxHandSize)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxHandSize, OldMaxHandSize);
}

void UCharacterAttributeSetBase::OnRep_SpellSlots(const FGameplayAttributeData& OldSpellSlots)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, SpellSlots, OldSpellSlots);
}

void UCharacterAttributeSetBase::OnRep_MaxSpellSlots(const FGameplayAttributeData& OldMaxSpellSlots)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxSpellSlots, OldMaxSpellSlots);
}

void UCharacterAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, HandSize, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxHandSize, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, SpellSlots, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxSpellSlots, COND_None, REPNOTIFY_Always);
}