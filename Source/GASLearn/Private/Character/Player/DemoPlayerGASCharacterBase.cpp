// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DemoPlayerGASCharacterBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GASLearn/GASLearn.h"
#include "GASLearn/GAS/Character/Ability/CharacterAbilitySystemComponent.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MyPlayerState.h"
#include "Engine.h"

// Sets default values
ADemoPlayerGASCharacterBase::ADemoPlayerGASCharacterBase(const class FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->FieldOfView = 80.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = APlayerController::StaticClass();
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

	OnActionTag = FGameplayTag::RequestGameplayTag(FName("SpecialTag.ActionState.OnAction"));
}

void ADemoPlayerGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	StartingCameraBoomLength = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			for(FIMC_Priority IMC_Priority: IMC_List)
			{
				if(IMC_Priority.IMC)
				{
					Subsystem->AddMappingContext(IMC_Priority.IMC, IMC_Priority.Priority);
				}
			}
		}
	}
}

// Called to bind functionality to input
void ADemoPlayerGASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogInput, Warning, TEXT("USING ENHANCED INPUT COMPONENT"));

		BindEnhancedInputComponent = EnhancedInputComponent;
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction.Get(), ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction.Get(), ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::Interact);

		for(UInputAction * IA: ActionIAs)
		{

			BindActionInputAction(IA);
		}

		for (TPair<TSubclassOf<UActionGameplayAbility>, TArray<TObjectPtr<UInputAction>>>& Pair : ActionAbilityToInputActionMap)
		{
			for(TObjectPtr<UInputAction> IA: Pair.Value)
			{
				BindActionInputAction(IA);
			}
		}

		
	}else
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &ADemoPlayerGASCharacterBase::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ADemoPlayerGASCharacterBase::MoveRight);
		PlayerInputComponent->BindAxis("LookUp", this, &ADemoPlayerGASCharacterBase::LookUp);
		PlayerInputComponent->BindAxis("LookUpRate", this, &ADemoPlayerGASCharacterBase::LookUpRate);
		PlayerInputComponent->BindAxis("Turn", this, &ADemoPlayerGASCharacterBase::Turn);
		PlayerInputComponent->BindAxis("TurnRate", this, &ADemoPlayerGASCharacterBase::TurnRate);
	}
	
	

	BindASCInput();
}

void ADemoPlayerGASCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);

		AddStartupEffects();

		AddCharacterAbilities();
	}
}

USpringArmComponent* ADemoPlayerGASCharacterBase::GetCameraBoom() const
{
	return CameraBoom;
}

UCameraComponent* ADemoPlayerGASCharacterBase::GetFollowCamera() const
{
	return FollowCamera;
}

float ADemoPlayerGASCharacterBase::getStartingCameraBoomArmLength()
{
	return StartingCameraBoomLength;
}

FVector ADemoPlayerGASCharacterBase::getStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}

void ADemoPlayerGASCharacterBase::LookUp(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value);
	}
}

void ADemoPlayerGASCharacterBase::LookUpRate(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ADemoPlayerGASCharacterBase::Turn(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void ADemoPlayerGASCharacterBase::TurnRate(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ADemoPlayerGASCharacterBase::MoveForward(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);
}

void ADemoPlayerGASCharacterBase::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);
}

void ADemoPlayerGASCharacterBase::UseActionGameplayAbility(const FInputActionInstance& InputInstance)
{
	const UInputAction* Action = InputInstance.GetSourceAction();
	if (AbilitySystemComponent.IsValid())
	{
		if(AbilitySystemComponent->HasMatchingGameplayTag(OnActionTag))
		{
			FGameplayAbilitySpecHandle * comboSpec = InputActionToComboAbilityMap.Find(Action);
			if (comboSpec)
			{
				AbilitySystemComponent->TryActivateAbility(*comboSpec, true);
			}
		}
		else
		{
			FGameplayAbilitySpecHandle * basicSpec = InputActionToBasicAbilityMap.Find(Action);
			if (basicSpec)
			{
				AbilitySystemComponent->TryActivateAbility(*basicSpec, true);
			}else
			{
				UE_LOG(LogInput, Warning, TEXT("InputActionToAbilityMap does not contain %s"), *Action->GetFName().ToString());
			}
		}

		if (InputActionToAlwaysAbilityMap.Contains(Action))
		{
			FGameplayAbilitySpecHandle * alwaysSpec = InputActionToAlwaysAbilityMap.Find(Action);
			if (alwaysSpec)
			{
				AbilitySystemComponent->TryActivateAbility(*alwaysSpec, true);
			}
		}
	}else
	{
		UE_LOG(LogInput, Warning, TEXT("AbilitySystemComponent is not valid"));
	}
	
}

bool ADemoPlayerGASCharacterBase::onAddActionGameplayAbility(TSubclassOf<UActionGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	if (Ability && GetLocalRole() == ROLE_Authority && AbilitySystemComponent.IsValid())
	{
	
		ActionAbilityToSpec.Add(Ability, AbilitySpecHandle);

		TArray<TObjectPtr<UInputAction>> AbilityInputActions;
		
		for(UInputAction * IA: Ability->GetDefaultObject<UActionGameplayAbility>()->BasicInputActions)
		{
			BindActionInputAction(IA);
			
			if(!InputActionToBasicAbilityMap.Contains(IA))
			{
				InputActionToBasicAbilityMap.Add(IA, AbilitySpecHandle);
			}else
			{
				FString basicName = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability.GetName();
				UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s when add ability: %s"), *IA->GetName(), *basicName, *Ability->GetName());
			}
			AbilityInputActions.Add(IA);
		}

		for (UInputAction * IA: Ability->GetDefaultObject<UActionGameplayAbility>()->ComboInputActions)
		{
			BindActionInputAction(IA);
			AbilityInputActions.Add(IA);
		}

		for (UInputAction * IA: Ability->GetDefaultObject<UActionGameplayAbility>()->AlwaysInputActions)
		{
			BindActionInputAction(IA);

			if(!InputActionToAlwaysAbilityMap.Contains(IA))
			{
				InputActionToAlwaysAbilityMap.Add(IA, AbilitySpecHandle);
			}else
			{
				FString basicName = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability.GetName();
				UE_LOG(LogInput, Warning, TEXT("InputActionToAlwaysAbilityMap already contains action %s, ability: %s when add ability: %s"), *IA->GetName(), *basicName, *Ability->GetName());
			}
			AbilityInputActions.Add(IA);
		}

		ActionAbilityToInputActionMap.Add(Ability, AbilityInputActions);

		return true;
	}

	return false;
}

void ADemoPlayerGASCharacterBase::DeActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility> Ability, TArray<UInputAction*> InputActions)
{
	if (ActionAbilityToSpec.Contains(Ability))
	{
		FGameplayAbilitySpecHandle AbilitySpecHandle = ActionAbilityToSpec[Ability];
		
		for(UInputAction * InputAction: InputActions)
		{
			if(InputActionToComboAbilityMap.Find(InputAction))
			{
				if (InputActionToComboAbilityMap[InputAction] != AbilitySpecHandle)
				{
					UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s"), *InputAction->GetName(), *Ability->GetName());
				}else
				{
					InputActionToComboAbilityMap.Remove(InputAction);
				}
			}
			else
			{
				UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap not contains action %s, ability: %s"), *InputAction->GetName(), *Ability->GetName());
			}
		}
	}else
	{
		UE_LOG(LogInput, Warning, TEXT("ActionAbilityToSpec does not contain ability %s"), *Ability->GetName());
	}
}

void ADemoPlayerGASCharacterBase::ActiveActionGameplayAbilityComboInput(TSubclassOf<UActionGameplayAbility>Ability, TArray<UInputAction*> InputActions)
{
	if (ActionAbilityToSpec.Contains(Ability))
	{
		FGameplayAbilitySpecHandle AbilitySpecHandle = ActionAbilityToSpec[Ability];
		
		for(UInputAction * InputAction: InputActions)
		{
			if(! BindedInputActions.Contains(InputAction))
			{
				
			}
			
			if(!InputActionToComboAbilityMap.Find(InputAction))
			{
				InputActionToComboAbilityMap.Add(InputAction, AbilitySpecHandle);
			}
			else
			{
				UE_LOG(LogInput, Warning, TEXT("InputActionToBasicAbilityMap already contains action %s, ability: %s"), *InputAction->GetName(), *Ability->GetName());
			}
		}
	}else
	{
		UE_LOG(LogInput, Warning, TEXT("ActionAbilityToSpec does not contain ability %s"), *Ability->GetName());
	}
}

void ADemoPlayerGASCharacterBase::BindActionInputAction(TObjectPtr<UInputAction> IA)
{
	if(!BindedInputActions.Contains(IA) && BindEnhancedInputComponent)
	{
		BindEnhancedInputComponent->BindAction(IA, ETriggerEvent::Completed, this, &ADemoPlayerGASCharacterBase::UseActionGameplayAbility);
		
		BindedInputActions.Add(IA);
	}
}


void ADemoPlayerGASCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);
		
		BindASCInput();
		
	}
}

void ADemoPlayerGASCharacterBase::InitializeStartingValues(AMyPlayerState* PS)
{
	AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSetBase = PS->GetAttributeSetBase();

	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	BindAttributeDelegates();
	InitialAttributes();
}

void ADemoPlayerGASCharacterBase::Move(const FInputActionValue& Value)
{
	if(IsAlive())
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ADemoPlayerGASCharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADemoPlayerGASCharacterBase::Interact(const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact!"));
	}

}

void ADemoPlayerGASCharacterBase::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		const FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/GASLearn"), FName("EDemoAbilityID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,
			FGameplayAbilityInputBinds(FString("ConfirmTarget"),FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EDemoAbilityID::Confirm), static_cast<int32>(EDemoAbilityID::Cancel)));
		ASCInputBound = true;
	}
}

void ADemoPlayerGASCharacterBase::GetCameraParam_Implementation(float& TpFov, float& FpFov, bool& RightShoulder)
{
	TpFov = CameraTpFov;
	FpFov = CameraFpFov;
	RightShoulder = CameraRightShoulder;
}

void ADemoPlayerGASCharacterBase::GetFPCameraTarget_Implementation(FVector& FpCameraTarget)
{
	FpCameraTarget = GetMesh()->GetSocketLocation(FName("FP_Camera"));
}

void ADemoPlayerGASCharacterBase::GetTPPivotTarget_Implementation(FTransform& TpCameraTarget)
{
	FVector Head = GetMesh()->GetSocketLocation(FName("Head"));
	FVector Root = GetMesh()->GetSocketLocation(FName("Root"));

	TpCameraTarget = FTransform(GetActorRotation(), (Head + Root) / 2.0f, FVector(1.0f));
	
}

void ADemoPlayerGASCharacterBase::GetTPTraceParams_Implementation(FVector& TraceOrigin, float& TraceRadius, TEnumAsByte<ETraceTypeQuery>& TraceChannel)
{
	if (CameraRightShoulder)
	{
		TraceOrigin = GetMesh()->GetSocketLocation(FName("TP_CameraTrace_R"));
	}else
	{
		TraceOrigin = GetMesh()->GetSocketLocation(FName("TP_CameraTrace_L"));
	}
	TraceRadius = TPTraceRadius;
	TraceChannel = TPTraceChannel;
}