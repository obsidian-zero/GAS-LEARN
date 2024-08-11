// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EActionGAType : uint8
{
	Basic UMETA(DisplayName = "Basic"),
	Combo UMETA(DisplayName = "Combo")
};