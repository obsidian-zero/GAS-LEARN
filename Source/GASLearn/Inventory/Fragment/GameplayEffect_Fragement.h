// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/Inventory/MeteorInventoryFragmentBase.h"
#include "GameplayEffect_Fragement.generated.h"

UCLASS()
class GASLEARN_API UGameplayEffect_Fragement : public UMeteorInventoryFragmentBase
{
	GENERATED_BODY()

public:
	UGameplayEffect_Fragement(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnItemInstanceOwnerChanged(UMeteorInventoryItemInstance* ItemInstance, AActor* OldOwner, AActor* NewOwner);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameplayEffect")
	TArray<TSubclassOf<class UGameplayEffect>> EquippedGameplayEffects;

	virtual void onItemInstancedCreated_Implementation(UMeteorInventoryItemInstance* ItemInstance) override;
};
