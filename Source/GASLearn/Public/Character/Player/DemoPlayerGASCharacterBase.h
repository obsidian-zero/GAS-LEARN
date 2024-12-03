// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GASCharacterBase.h"
#include "GASLearn/Public/Player/MyPlayerState.h"
#include "InputActionValue.h"
#include "StructDefine.h"
#include "GASLearn/GAS/Ability/ActionGameplayAbility.h"
#include "GASLearn/Public/Character/Abilities/CharacterGameplayAbility.h"
#include "Templates/SharedPointer.h"
#include "GASLearn/Camera/MeteorCameraInterface.h"
#include "Engine/EngineTypes.h"

#include "DemoPlayerGASCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API ADemoPlayerGASCharacterBase : public AGASCharacterBase, public IMeteorCameraInterface
{
	GENERATED_BODY()
public:
	ADemoPlayerGASCharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	TObjectPtr<UEnhancedInputComponent> BindEnhancedInputComponent;
	
	class USpringArmComponent* GetCameraBoom() const;

	class UCameraComponent* GetFollowCamera() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|Camera")
	float getStartingCameraBoomArmLength();

	UFUNCTION(BlueprintCallable, Category = "Demo|Camera")
	FVector getStartingCameraBoomLocation();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Input")
	TArray<FIMC_Priority> IMC_List;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Demo|Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Demo|Input|Action", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Demo|Input|Action", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Demo|Input|Action", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Input|Action")
	TArray<UInputAction *> ActionIAs;

	TArray<TObjectPtr<UInputAction>> BindedInputActions;

	void BindActionInputAction(TObjectPtr<UInputAction>);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Camera")
	float BaseTurnRate = 45.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Camera")
	float BaseLookUpRate = 45.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Demo|Camera")
	float StartingCameraBoomLength;

	UPROPERTY(BlueprintReadOnly, Category = "Demo|Camera")
	FVector StartingCameraBoomLocation;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Demo|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Demo|Camera")
	class UCameraComponent* FollowCamera;

	bool ASCInputBound = false;

	FGameplayTag DeadTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meteor|Action")
	FGameplayTag OnActionTag;

	// 输入动作对应的基本尝试触发技能
	TMap<UInputAction *, FGameplayAbilitySpecHandle> InputActionToBasicAbilityMap;

	// 输入动作对应的连击尝试触发技能
	TMap<UInputAction *, FGameplayAbilitySpecHandle> InputActionToComboAbilityMap;

	// 输入动作对应的永久尝试触发技能
	TMap<UInputAction *, FGameplayAbilitySpecHandle> InputActionToAlwaysAbilityMap;

	// 技能对应的全部输入动作
	TMap<TSubclassOf<UActionGameplayAbility>, TArray<TObjectPtr<UInputAction>>> ActionAbilityToInputActionMap;

	TMap<TSubclassOf<UActionGameplayAbility>, FGameplayAbilitySpecHandle> ActionAbilityToSpec;
	
	virtual void BeginPlay() override;
	
	void LookUp(float Value);

	void LookUpRate(float Value);

	void Turn(float Value);

	void TurnRate(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void UseActionGameplayAbility(const FInputActionInstance& Instance);
	
	bool onAddActionGameplayAbility(TSubclassOf<UActionGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle);

	void ActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction *> InputActions);

	void DeActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction *> InputActions);
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void Interact(const FInputActionValue& Value);
	
	virtual void OnRep_PlayerState() override;

	void BindASCInput();

	void InitializeStartingValues(AMyPlayerState* PS);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem")
	float CameraTpFov = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem")
	float CameraFpFov = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem")
	bool CameraRightShoulder = true;
	
	virtual void GetCameraParam_Implementation(float& TpFov, float& FpFov, bool& RightShoulder) override;

	virtual void GetFPCameraTarget_Implementation(FVector& FpCameraTarget) override;

	virtual void GetTPPivotTarget_Implementation(FTransform& TpCameraTarget) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem")
	float TPTraceRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSystem")
	TEnumAsByte<ECollisionChannel> TPTraceChannel;
	
	virtual void GetTPTraceParams_Implementation(FVector& TraceOrigin, float& TraceRadius, TEnumAsByte<ECollisionChannel>& TraceChannel) override;

};
