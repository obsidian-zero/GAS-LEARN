// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/Inventory/MeteorInventoryFragmentBase.h"
#include "MeteorStaticMesh_FragmentBase.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorStaticMesh_FragmentBase : public UMeteorInventoryFragmentBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Mesh", Replicated)
	TSoftObjectPtr<UStaticMesh> Mesh;
	
};
