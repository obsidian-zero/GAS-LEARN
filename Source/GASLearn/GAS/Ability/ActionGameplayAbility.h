// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/GAS/Ability/MeteorGameplayAbility.h"
#include "EnumDefine.h"
#include "ActionGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API UActionGameplayAbility : public UMeteorGameplayAbility
{
	GENERATED_BODY()

public:

	/** 技能的永久输入动作组 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionInfo", meta = (AllowPrivateAccess = "true", ToolTip = "这些操作时时总是尝试激活技能"))
	TArray<UInputAction*> AlwaysInputActions;
	
	/** 技能的基本输入动作组 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionInfo", meta = (AllowPrivateAccess = "true", ToolTip = "这些操作时在不在其他动作时会尝试激活技能"))
	TArray<UInputAction*> BasicInputActions;

	/** 技能的连击输入动作组 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionInfo", meta = (AllowPrivateAccess = "true", ToolTip = "这些操作时在其他动作时会尝试激活技能"))
	TArray<UInputAction*> ComboInputActions;

	//** 技能的输入优先级 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionInfo")
	int32 InputPriority;

	UFUNCTION(BlueprintCallable, Category = "Action")
	virtual int32 GetInputPriority() const { return InputPriority; }
};
