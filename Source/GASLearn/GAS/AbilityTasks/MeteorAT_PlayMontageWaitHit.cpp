// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/AbilityTasks/MeteorAT_PlayMontageWaitHit.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemGlobals.h"
#include "GASLearn/HitTraceComponent/HitTraceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MeteorAT_PlayMontageWaitHit)

static bool GUseAggressivePlayMontageAndWaitEndTask = true;
static FAutoConsoleVariableRef CVarAggressivePlayMontageAndWaitEndTask(TEXT("AbilitySystem.PlayMontage.AggressiveEndTask"), GUseAggressivePlayMontageAndWaitEndTask, TEXT("This should be set to true in order to avoid multiple callbacks off an AbilityTask_PlayMontageAndWait node"));

void UMeteorAT_PlayMontageWaitHit::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	const bool bPlayingThisMontage = (Montage == MontageToPlay) && Ability && Ability->GetCurrentMontage() == MontageToPlay;
	if (bPlayingThisMontage)
	{
		// Reset AnimRootMotionTranslationScale
		ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
		if (Character && (Character->GetLocalRole() == ROLE_Authority ||
							(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
		{
			Character->SetAnimRootMotionTranslationScale(1.f);
		}
	}

	if (bPlayingThisMontage && (bInterrupted || !bAllowInterruptAfterBlendOut))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->ClearAnimatingAbility(Ability);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (bInterrupted)
		{
			OnInterrupted.Broadcast(FHitResult(), "", nullptr);

			if (GUseAggressivePlayMontageAndWaitEndTask)
			{
				EndTask();
			}
		}
		else
		{
			OnBlendOut.Broadcast(FHitResult(), "", nullptr);
		}
	}
}

void UMeteorAT_PlayMontageWaitHit::OnGameplayAbilityCancelled()
{
	if (StopPlayingMontage() || bAllowInterruptAfterBlendOut)
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FHitResult(), "", nullptr);;
		}
	}

	if (GUseAggressivePlayMontageAndWaitEndTask)
	{
		EndTask();
	}
}

void UMeteorAT_PlayMontageWaitHit::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FHitResult(), "", nullptr);;
		}
	}

	EndTask();
}

void UMeteorAT_PlayMontageWaitHit::OnHitComponentCreateHitTrace(const FHitResult& HitResult, FString TaskName, AActor* HitActor)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnHitTrace.Broadcast(HitResult, TaskName, HitActor);
	}
}

UMeteorAT_PlayMontageWaitHit* UMeteorAT_PlayMontageWaitHit::CreatePlayMontageAndWaitProxy(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	UAnimMontage* MontageToPlay,
	float Rate,
	FName StartSection,
	bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale,
	float StartTimeSeconds,
	bool bAllowInterruptAfterBlendOut)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UMeteorAT_PlayMontageWaitHit* MyObj = NewAbilityTask<UMeteorAT_PlayMontageWaitHit>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterruptAfterBlendOut;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}


void UMeteorAT_PlayMontageWaitHit::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}
	
	bool bPlayedMontage = false;

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();

		if(AActor * AvatarActor = ActorInfo->AvatarActor.Get())
		{
			if(UHitTraceComponent* HitTraceComponent = AvatarActor->FindComponentByClass<UHitTraceComponent>())
			{
				HitTraceComponent->OnHitDetected.AddDynamic(this, &UMeteorAT_PlayMontageWaitHit::OnHitComponentCreateHitTrace);
			}
		}
		
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
			{
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UMeteorAT_PlayMontageWaitHit::OnGameplayAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UMeteorAT_PlayMontageWaitHit::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UMeteorAT_PlayMontageWaitHit::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
								  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			ABILITY_LOG(Warning, TEXT("UMeteorAT_PlayMontageWaitHit call to PlayMontage failed!"));
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UMeteorAT_PlayMontageWaitHit called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		ABILITY_LOG(Warning, TEXT("UMeteorAT_PlayMontageWaitHit called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay),*InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FHitResult(), "", nullptr);;
		}
	}

	SetWaitingOnAvatar();
}

void UMeteorAT_PlayMontageWaitHit::ExternalCancel()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast(FHitResult(), "", nullptr);;
	}
	Super::ExternalCancel();
}

void UMeteorAT_PlayMontageWaitHit::OnDestroy(bool AbilityEnded)
{
	// Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
	// (If we are destroyed, it will detect this and not do anything)

	// This delegate, however, should be cleared as it is a multicast
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(InterruptedHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();

		if(AActor * AvatarActor = ActorInfo->AvatarActor.Get())
		{
			if(UHitTraceComponent* HitTraceComponent = AvatarActor->FindComponentByClass<UHitTraceComponent>())
			{
				HitTraceComponent->OnHitDetected.RemoveDynamic(this, &UMeteorAT_PlayMontageWaitHit::OnHitComponentCreateHitTrace);
			}
		}
	}

	Super::OnDestroy(AbilityEnded);
}

bool UMeteorAT_PlayMontageWaitHit::StopPlayingMontage()
{
	if (Ability == nullptr)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (ASC->GetAnimatingAbility() == Ability
			&& ASC->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

FString UMeteorAT_PlayMontageWaitHit::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? ToRawPtr(MontageToPlay) : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWait. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

