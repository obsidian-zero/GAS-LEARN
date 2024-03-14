// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyPlayerState();

	class UAbilitySystemComponent * GetAbilitySystemComponent() const override;
	class UCharacterAttributeSetBase * GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState")
	bool IsAlive() const;


	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetHandSize() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetMaxHandSize() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetSpellSlots() const;
	
	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetMaxSpellSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UCharacterAbilitySystemComponent * AbilitySystemComponent;

	UPROPERTY()
	class UCharacterAttributeSetBase * AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HandSizeChangedDelegateHandle;
	FDelegateHandle MaxHandSizeChangedDelegateHandle;
	FDelegateHandle SpellSlotsChangedDelegateHandle;
	FDelegateHandle MaxSpellSlotsChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HandSizeChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHandSizeChanged(const FOnAttributeChangeData& Data);
	virtual void SpellSlotsChanged(const FOnAttributeChangeData& Data);
	virtual void MaxSpellSlotsChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallBackTag, int32 NewCount);
	
	
};
