// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Entity/EntityCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"


AEntityCharacterBase::AEntityCharacterBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// 创建能力系统组件
	EntityAbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("EntityAbilitySystemComponent"));
    
	// 创建属性集组件
	EntityAttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("EntityAttributeSetBase"));
}

void AEntityCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(EntityAbilitySystemComponent);
	
	AttributeSetBase = EntityAttributeSetBase;

	EntityAbilitySystemComponent->InitAbilityActorInfo(this, this);

	BindAttributeDelegates();
	InitialAttributes();
}