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
	return GetHandSize() <= GetMaxHandSize();
}

void AMyPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	// todo		
}

float AMyPlayerState::GetHandSize() const
{
	return AttributeSetBase->GetHandSize();
}

float AMyPlayerState::GetMaxHandSize() const
{
	return AttributeSetBase->GetMaxHandSize();
}

float AMyPlayerState::GetSpellSlots() const
{
	return AttributeSetBase->GetSpellSlots();
}

float AMyPlayerState::GetMaxSpellSlots() const
{
	return AttributeSetBase->GetMaxSpellSlots();
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
		// 绑定 HandSizeChanged 函数到手牌大小属性改变的委托上
		HandSizeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHandSizeAttribute()).AddUObject(this, &AMyPlayerState::HandSizeChanged);

		// 绑定 MaxHandSizeChanged 函数到最大手牌大小属性改变的委托上
		MaxHandSizeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHandSizeAttribute()).AddUObject(this, &AMyPlayerState::MaxHandSizeChanged);

		// 绑定 SpellSlotsChanged 函数到法术槽属性改变的委托上
		SpellSlotsChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetSpellSlotsAttribute()).AddUObject(this, &AMyPlayerState::SpellSlotsChanged);

		// 绑定 MaxSpellSlotsChanged 函数到最大法术槽属性改变的委托上
		MaxSpellSlotsChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxSpellSlotsAttribute()).AddUObject(this, &AMyPlayerState::MaxSpellSlotsChanged);

		// 绑定 CharacterLevelChanged 函数到角色等级属性改变的委托上
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetLevelAttribute()).AddUObject(this, &AMyPlayerState::CharacterLevelChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.DeBuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMyPlayerState::StunTagChanged);


	}
}


// Implement the function HandSizeChanged
void AMyPlayerState:: HandSizeChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Error, TEXT("HAND SIZE CHANGE"))
}

// Implement the function MaxHandSizeChanged
void AMyPlayerState:: MaxHandSizeChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Error, TEXT("MAX HAND SIZE CHANGE"))
}

// Implement the function SpellSlotsChanged
void AMyPlayerState:: SpellSlotsChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Error, TEXT("SPELL SLOTS CHANGE"))
}

// Implement the function MaxSpellSlotsChanged
void AMyPlayerState:: MaxSpellSlotsChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Error, TEXT("MAX SPELL SLOTS CHANGE"))
}

// Implement the function CharacterLevelChanged
void AMyPlayerState:: CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Error, TEXT("CHARACTER LEVEL CHANGE"))
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