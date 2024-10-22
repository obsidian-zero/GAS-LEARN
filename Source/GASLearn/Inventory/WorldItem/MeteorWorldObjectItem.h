// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GASLearn/Inventory/MeteorInventoryItemInstance.h"
#include "MeteorWorldObjectItem.generated.h"

UCLASS()
class GASLEARN_API AMeteorWorldObjectItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AMeteorWorldObjectItem();

	UStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetInventoryInstance(UMeteorInventoryItemInstance* InInventoryInstance);
	virtual void SetInventoryInstance_Implementation(UMeteorInventoryItemInstance* InInventoryInstance);

	UFUNCTION(BlueprintPure, Category = "Item")
	virtual UMeteorInventoryItemInstance* GetInInventoryInstance();

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Item")
	UMeteorInventoryItemInstance* InventoryInstance;
};
