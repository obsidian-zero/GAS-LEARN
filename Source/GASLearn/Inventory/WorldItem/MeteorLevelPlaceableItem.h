// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorLevelPlaceableItem.generated.h"

UCLASS()
class GASLEARN_API AMeteorLevelPlaceableItem : public AActor
{
	GENERATED_BODY()
	
private:	
	UPROPERTY()
	UStaticMeshComponent* EditorStaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AMeteorLevelPlaceableItem();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Item")
	bool bSpawnItemOnBeginPlay;

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual bool SpawnWorldItem();

	UFUNCTION()
	virtual void OnSpawnedItemStackConsumed(AActor* Actor);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
};
