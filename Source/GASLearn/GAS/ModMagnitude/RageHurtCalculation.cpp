// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/ModMagnitude/RageHurtCalculation.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"

URageHurtCalculation::URageHurtCalculation()
{
	RageSelfDef.AttributeToCapture = UCharacterAttributeSetBase::GetRageAttribute();
	RageSelfDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	RageSelfDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(RageSelfDef);

	RageEnermyDef.AttributeToCapture = UCharacterAttributeSetBase::GetRageAttribute();
	RageEnermyDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	RageEnermyDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(RageEnermyDef);
}

float URageHurtCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float SelfRage = 0.f;
	GetCapturedAttributeMagnitude(RageSelfDef, Spec, EvaluationParameters, SelfRage);

	float EnermyRage = 0.f;
	GetCapturedAttributeMagnitude(RageEnermyDef, Spec, EvaluationParameters, EnermyRage);

	return FMath::Max(10.0f, SelfRage - EnermyRage);
}