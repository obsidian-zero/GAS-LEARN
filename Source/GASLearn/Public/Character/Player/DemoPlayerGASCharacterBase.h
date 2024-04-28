// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GASCharacterBase.h"
#include "GASLearn/Public/Player/MyPlayerState.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "StructDefine.h"
#include "GASLearn/Public/Character/Abilities/CharacterGameplayAbility.h"
#include "Templates/SharedPointer.h"


#include "DemoPlayerGASCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class GASLEARN_API ADemoPlayerGASCharacterBase : public AGASCharacterBase
{
	GENERATED_BODY()
public:
	ADemoPlayerGASCharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TArray<FIA_GA> InputAbilityList;

protected:
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
	
	TMap<UInputAction *, FGameplayAbilitySpecHandle> InputActionToAbilityMap;

	virtual void BeginPlay() override;
	
	void LookUp(float Value);

	void LookUpRate(float Value);

	void Turn(float Value);

	void TurnRate(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void UseAbilityInputAction(const FInputActionInstance& Instance);

	void AddInputAbilities();
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void OnRep_PlayerState() override;

	void BindASCInput();

	void InitializeStartingValues(AMyPlayerState* PS);
};
