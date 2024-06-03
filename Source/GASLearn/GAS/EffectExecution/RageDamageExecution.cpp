// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/EffectExecution/RageDamageExecution.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"

struct RageDamageSourceStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Rage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxRage);

	RageDamageSourceStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Rage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, MaxRage, Source, true);
	}
};

struct RageDamagaTargetStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Rage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxRage);

	RageDamagaTargetStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Rage, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, MaxRage, Target, true);
	}
};

URageDamageExecution::URageDamageExecution()
{
	RelevantAttributesToCapture.Add(RageDamageSourceStatics().RageDef);
	RelevantAttributesToCapture.Add(RageDamageSourceStatics().MaxRageDef);

	RelevantAttributesToCapture.Add(RageDamagaTargetStatics().RageDef);
	RelevantAttributesToCapture.Add(RageDamagaTargetStatics().MaxRageDef);
}


void URageDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvaluationParameters;

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float SourceRage = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(RageDamageSourceStatics().RageDef, EvaluationParameters, SourceRage);

	float TargetRage = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(RageDamagaTargetStatics().RageDef, EvaluationParameters, TargetRage);

	// 对来源和目标的怒气进行比较，然后减去较小的那个
	if(SourceRage > TargetRage)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RageDamagaTargetStatics().RageProperty, EGameplayModOp::Additive, -TargetRage));
	}
	else
	{
		
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RageDamagaTargetStatics().RageProperty, EGameplayModOp::Additive, -SourceRage));
		// 这里注意，虽然是减去SourceRageStatics，但是实际上是减去Target，因为本质上是对GE的效果增加了一个属性维度的修改, 并不因为source和target而有所不同
		// OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RageDamageSourceStatics().RageProperty, EGameplayModOp::Additive, -SourceRage));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("URageDamageExecution::Execute_Implementation"));
}