// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerState.h"

#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "GASLearn/GAS/Character/Ability/CharacterAbilitySystemComponent.h"

AMyPlayerState::AMyPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSetBase* AMyPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AMyPlayerState::IsAlive() const
{
	return GetHealth() <= GetMaxHealth();
}

void AMyPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	// todo		
}

float AMyPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AMyPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AMyPlayerState::GetRage() const
{
	return AttributeSetBase->GetRage();
}

float AMyPlayerState::GetMaxRage() const
{
	return AttributeSetBase->GetMaxRage();
}

float AMyPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetLevel();
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AMyPlayerState::HealthChanged);
		
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AMyPlayerState::MaxHealthChanged);

		RageChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetRageAttribute()).AddUObject(this, &AMyPlayerState::RageChanged);
		
		MaxRageChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxRageAttribute()).AddUObject(this, &AMyPlayerState::MaxRageChanged);

		// 绑定 CharacterLevelChanged 函数到角色等级属性改变的委托上
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetLevelAttribute()).AddUObject(this, &AMyPlayerState::CharacterLevelChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.DeBuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMyPlayerState::StunTagChanged);


	}
}


void AMyPlayerState:: HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("HEALTH CHANGE"))
}

void AMyPlayerState:: MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("MAX HEALTH CHANGE"))
}

void AMyPlayerState:: RageChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("RAGE CHANGE"))
}

void AMyPlayerState:: MaxRageChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("MAX RAGE CHANGE"))
}

// Implement the function CharacterLevelChanged
void AMyPlayerState:: CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("CHARACTER LEVEL CHANGE"))
}

// Implement the function StunTagChanged
void AMyPlayerState:: StunTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}