// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASLearn/GAS/AbilityTasks/MeteorAbilityTaskBase.h"
#include "MeteorAT_PlayMontageWaitHit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMontageWaitHitTraceDelegate, const FHitResult&, HitResult, FString, TaskName, AActor *, HitActor);

/**
 * 
 */
UCLASS()
class GASLEARN_API UMeteorAT_PlayMontageWaitHit : public UMeteorAbilityTaskBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FMontageWaitHitTraceDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitHitTraceDelegate	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitHitTraceDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitHitTraceDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitHitTraceDelegate OnHitTrace;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnHitComponentCreateHitTrace(const FHitResult& HitResult, FString TaskName, AActor * HitActor);
	

	/** 
	 * Start playing an animation montage on the avatar actor and wait for it to finish
	 * If StopWhenAbilityEnds is true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled.
	 * On normal execution, OnBlendOut is called when the montage is blending out, and OnCompleted when it is completely done playing
	 * OnInterrupted is called if another montage overwrites this, and OnCancelled is called if the ability or task is cancelled
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param MontageToPlay The montage to play on the character
	 * @param Rate Change to play the montage faster or slower
	 * @param StartSection If not empty, named montage section to start from
	 * @param bStopWhenAbilityEnds If true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled
	 * @param AnimRootMotionTranslationScale Change to modify size of root motion or set to 0 to block it entirely
	 * @param StartTimeSeconds Starting time offset in montage, this will be overridden by StartSection if that is also set
	 * @param bAllowInterruptAfterBlendOut If true, you can receive OnInterrupted after an OnBlendOut started (otherwise OnInterrupted will not fire when interrupted, but you will not get OnComplete).
	*/
	UFUNCTION(BlueprintCallable, Category="Ability|MeteorAT", 
	meta = (DisplayName="PlayMontageAndWaitHit",
	HidePin = "OwningAbility", 
	DefaultToSelf = "OwningAbility", 
	BlueprintInternalUseOnly = "TRUE"))
	static UMeteorAT_PlayMontageWaitHit* CreatePlayMontageAndWaitProxy(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.f,
		float StartTimeSeconds = 0.f,
		bool bAllowInterruptAfterBlendOut = false);   


	virtual void Activate() override;

	/** Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
	virtual void ExternalCancel() override;

	virtual FString GetDebugString() const override;

protected:

	virtual void OnDestroy(bool AbilityEnded) override;

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	bool bAllowInterruptAfterBlendOut;
};

