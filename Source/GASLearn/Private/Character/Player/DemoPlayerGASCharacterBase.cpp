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

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::Look);

		//Ability1
		EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::UseAbility1);

		//AbilityTag
		EnhancedInputComponent->BindAction(AbilityByTagAction, ETriggerEvent::Triggered, this, &ADemoPlayerGASCharacterBase::UseAbilityByTag);

		
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

		Ability1Spec = AddCharacterAbility(Ability1);
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

void ADemoPlayerGASCharacterBase::UseAbility1(const FInputActionValue& Value)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->TryActivateAbility(Ability1Spec, true);
	}
}

void ADemoPlayerGASCharacterBase::UseAbilityByTag(const FInputActionValue& Value)
{
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTag1, true);
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