// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/Inventory/Fragment/GameplayEffect_Fragement.h"
#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h" // 确保包含接口头文件

UGameplayEffect_Fragement::UGameplayEffect_Fragement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsDynamic = true;
}

void UGameplayEffect_Fragement::onItemInstancedCreated_Implementation(UMeteorInventoryItemInstance * ItemInstance)
{
	ItemInstance->OnItemInstanceOwnerChanged.AddDynamic(this, &UGameplayEffect_Fragement::OnItemInstanceOwnerChanged);
}

void UGameplayEffect_Fragement::OnItemInstanceOwnerChanged(UMeteorInventoryItemInstance * ItemInstance, AActor * OldOwner, AActor * NewOwner)
{
	// 从旧的 Owner 上移除效果
	if (OldOwner)
	{
		UAbilitySystemComponent* OldASC = nullptr;

		if (OldOwner->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			OldASC = Cast<IAbilitySystemInterface>(OldOwner)->GetAbilitySystemComponent();
		}

		if (OldASC)
		{
			// 从 OldOwner 上移除每个 GameplayEffect
			for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : EquippedGameplayEffects)
			{
				if (GameplayEffectClass)
				{
					// 移除指定类的效果
					OldASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, OldASC);
				}
			}
		}
	}
	
	if (NewOwner != nullptr)
	{
		UAbilitySystemComponent* ASC = nullptr;

		if (NewOwner && NewOwner->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			// 安全地获取 AbilitySystemComponent
			ASC = Cast<IAbilitySystemInterface>(NewOwner)->GetAbilitySystemComponent();
		}
		if (ASC)
		{
			// 将EquippedGameplayEffects中的每个GameplayEffect赋予NewOwner
			for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : EquippedGameplayEffects)
			{
				if (GameplayEffectClass)
				{
					// 构建GameplayEffectSpec
					FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
					FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);

					if (SpecHandle.IsValid())
					{
						// 应用GameplayEffect
						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		}
	}
}