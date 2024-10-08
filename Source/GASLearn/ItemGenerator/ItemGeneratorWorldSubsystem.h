// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemGeneratorWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UItemGeneratorWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	// 重写初始化方法
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 重写反初始化方法
	virtual void Deinitialize() override;

};
