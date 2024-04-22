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
	
	// 生命属性
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)

	// 生命值上限属性
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)

	// 定义怒气
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_Rage)
	FGameplayAttributeData Rage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Rage)

	// 定义怒气上限
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxRage)

	// 定义伤害元属性
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Damage)
	
	// 声明处理属性同步的函数
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Rage(const FGameplayAttributeData& OldRage);

	UFUNCTION()
	virtual void OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
