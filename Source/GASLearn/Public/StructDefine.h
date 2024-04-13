// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
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