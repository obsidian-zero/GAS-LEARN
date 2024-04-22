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
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetRage() const;
	
	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|DemoPlayerState|Attributes")
	float GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UCharacterAbilitySystemComponent * AbilitySystemComponent;

	UPROPERTY()
	class UCharacterAttributeSetBase * AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle RageChangedDelegateHandle;
	FDelegateHandle MaxRageChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void RageChanged(const FOnAttributeChangeData& Data);
	virtual void MaxRageChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallBackTag, int32 NewCount);
	
	
};
