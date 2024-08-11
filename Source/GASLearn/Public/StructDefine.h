// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GASLearn/GAS/Ability/MeteorGameplayAbility.h"

#include "StructDefine.generated.h"

/** 保存IMC的优先级顺序 */
USTRUCT(BlueprintType)
struct FIMC_Priority
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="IMC", MakeStructureDefaultValue="None"))
	TObjectPtr<UInputMappingContext> IMC = nullptr;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Priority", MakeStructureDefaultValue="0"))
	int32 Priority = 0;
};

/** 保存IA和GA的关系 */
USTRUCT(BlueprintType)
struct FIA_GA
{
	GENERATED_BODY()
public:
	/** 一个用于触发的InputAction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="IA", MakeStructureDefaultValue="None"))
	class  UInputAction* IA = nullptr;

	/** 一个使用后进行触发的GamplayAbility */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="GA", MakeStructureDefaultValue="None"))
	TSubclassOf<class UMeteorGameplayAbility> GA;
};