// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSetBase.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class GASLEARN_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	// 定义等级
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Level)
	
	// 定义手牌数量属性
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_HandSize)
	FGameplayAttributeData HandSize;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, HandSize)

	// 定义手牌上限属性
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_MaxHandSize)
	FGameplayAttributeData MaxHandSize;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHandSize)

	// 定义法术位数量属性
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_SpellSlots)
	FGameplayAttributeData SpellSlots;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, SpellSlots)

	// 定义法术位上限属性
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_MaxSpellSlots)
	FGameplayAttributeData MaxSpellSlots;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxSpellSlots)

	// 定义伤害元属性
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Damage)
	
	// 声明处理属性同步的函数
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);
	
	UFUNCTION()
	virtual void OnRep_HandSize(const FGameplayAttributeData& OldHandSize);

	UFUNCTION()
	virtual void OnRep_MaxHandSize(const FGameplayAttributeData& OldMaxHandSize);

	UFUNCTION()
	virtual void OnRep_SpellSlots(const FGameplayAttributeData& OldSpellSlots);

	UFUNCTION()
	virtual void OnRep_MaxSpellSlots(const FGameplayAttributeData& OldMaxSpellSlots);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
