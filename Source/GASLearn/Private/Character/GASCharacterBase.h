// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "GameplayAbilitySpecHandle.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "GASCharacterBase.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGASCharacterBase*, DiedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandSizeChangeDelegate, float, NewHandSize);
UCLASS()
class AGASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacterBase();
	
	AGASCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "Demo|Character|Delegate")
	FCharacterDiedDelegate OnCharacterDied;

	UPROPERTY(BlueprintAssignable, Category = "Demo|Character|Delegate")
	FHandSizeChangeDelegate OnHandSizeChangeDelegate;

	UFUNCTION(BlueprintCallable, Category = "Demo|Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|Character")
	virtual int32 GetAbilityLevel(EDemoAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Demo|Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attribute")
	float GetCharacterLevel() const;
	
	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attribute")
	float GetHandSize() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attribute")
	float GetMaxHandSize() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attribute")
	float GetSpellSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|Character|Attribute")
	float GetMaxSpellSlots() const;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Demo|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;

	virtual void AddCharacterAbilities();

	FGameplayAbilitySpecHandle AddCharacterAbility(TSubclassOf<class UCharacterGameplayAbility> Ability);

	virtual void InitialAttruibutes();

	virtual void AddStartupEffects();

	virtual void SetHandSize(float HandSize);

	virtual void SetSpellSlots(float SpellSlots);

	void OnHandSizeChanged(const FOnAttributeChangeData& Data);

	void BindAttributeDelegates();
};
