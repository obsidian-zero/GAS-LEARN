// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacterBase.h"

#include "Character/Abilities/CharacterGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "GASLearn/GAS/Character/Ability/CharacterAbilitySystemComponent.h"

// Sets default values
AGASCharacterBase::AGASCharacterBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

	bAlwaysRelevant = true;
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
}

UAbilitySystemComponent * AGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AGASCharacterBase::IsAlive() const
{
	return GetHealth() >= 0;
}

int32 AGASCharacterBase::GetAbilityLevel(EDemoAbilityID AbilityID) const
{
	return 1;
}

void AGASCharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities()) {
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0 ; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

float AGASCharacterBase::GetCharacterLevel() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetLevel();
	}
	return 0.0f;
}

float AGASCharacterBase::GetHealth() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float AGASCharacterBase::GetRage() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetRage();
	}
	return 0.0f;
}

float AGASCharacterBase::GetMaxHealth() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float AGASCharacterBase::GetMaxRage() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxRage();
	}
	return 0.0f;
}

void AGASCharacterBase::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void AGASCharacterBase::FinishDying()
{
	Destroy();
}

void AGASCharacterBase::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UMeteorGameplayAbility>& StartUpAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartUpAbility, 1, 1, this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

FGameplayAbilitySpecHandle AGASCharacterBase::AddCharacterAbility(TSubclassOf<UMeteorGameplayAbility> Ability)
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return FGameplayAbilitySpecHandle();
	}

	return AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 1, this));
}


void AGASCharacterBase::InitialAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	if(!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes."), *FString(__FUNCTION__));

	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContextHandle);

	if(NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AGASCharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		UE_LOG(LogTemp, Warning, TEXT("START UP EFFECTS NOT APPLIED"));
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContextHandle);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}
	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AGASCharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void AGASCharacterBase::SetRage(float Rage)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetRage(Rage);
	}
}

void AGASCharacterBase::OnHealthChanged(const FOnAttributeChangeData & Data)
{
	OnHealthChangeDelegate.Broadcast(Data.NewValue);
}

void AGASCharacterBase::OnMaxHealthChanged(const FOnAttributeChangeData & Data)
{
	OnMaxHealthChangeDelegate.Broadcast(Data.NewValue);
}

void AGASCharacterBase::OnRageChanged(const FOnAttributeChangeData & Data)
{
	OnRageChangeDelegate.Broadcast(Data.NewValue);
}

void AGASCharacterBase::OnMaxRageChanged(const FOnAttributeChangeData & Data)
{
	OnMaxRageChangeDelegate.Broadcast(Data.NewValue);
}

void AGASCharacterBase::BindAttributeDelegates()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGASCharacterBase::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGASCharacterBase::OnMaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetRageAttribute()).AddUObject(this, &AGASCharacterBase::OnRageChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxRageAttribute()).AddUObject(this, &AGASCharacterBase::OnMaxRageChanged);
	}
}


// Called when the game starts or when spawned
void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

