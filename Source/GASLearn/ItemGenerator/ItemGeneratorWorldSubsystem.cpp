// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/ItemGenerator/ItemGeneratorWorldSubsystem.h"

void UItemGeneratorWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// 初始化一些数据或状态
	UE_LOG(LogTemp, Warning, TEXT("ItemGeneratorSubsystem Initialized!"));
}

void UItemGeneratorWorldSubsystem::Deinitialize()
{
	// 清理资源或状态
	UE_LOG(LogTemp, Warning, TEXT("ItemGeneratorSubsystem Deinitialized!"));
	Super::Deinitialize();
}