// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestAssetManager.h"
#include "AbilitySystemGlobals.h"

void UMyTestAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	UE_LOG(LogTemp, Warning, TEXT("hello world"));
}
