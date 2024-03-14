// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DemoPlayerGASCharacterBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GASLearn/GASLearn.h"
#include "GASLearn/GAS/Character/Ability/CharacterAbilitySystemComponent.h"
#include "GASLearn/GAS/Attributes/CharacterAttributeSetBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/MyPlayerState.h"

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
}

// Called to bind functionality to input
void ADemoPlayerGASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADemoPlayerGASCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADemoPlayerGASCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ADemoPlayerGASCharacterBase::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADemoPlayerGASCharacterBase::LookUpRate);
	PlayerInputComponent->BindAxis("Turn", this, &ADemoPlayerGASCharacterBase::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADemoPlayerGASCharacterBase::TurnRate);

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

void ADemoPlayerGASCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);
		BindASCInput();
		InitialAttruibutes();
	}
}

void ADemoPlayerGASCharacterBase::InitializeStartingValues(AMyPlayerState* PS)
{
	AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSetBase = PS->GetAttributeSetBase();

	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	SetHandSize(0);
	SetSpellSlots(0);
}

void ADemoPlayerGASCharacterBase::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/GASLearn"), FName("DemoAbilityID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(DemoAbilityID::Confirm), static_cast<int32>(DemoAbilityID::Cancel)));
	}
}