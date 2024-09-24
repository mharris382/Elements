// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mage.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Player/ElementsPlayerController.h"
#include "Player/ElementsPlayerState.h"
#include "InputActionValue.h"
#include "ElementsGameMode.h"




AMage::AMage(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	

}

void AMage::BeginPlay()
{
	Super::BeginPlay();
}

void AMage::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AElementsPlayerState* PS = GetPlayerState<AElementsPlayerState>();
	if (PS) 
	{
		
		AbilitySystemComponent = Cast<UElementsAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		//PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, this);

		AttributeSetBase = PS->GetAttributeSetBase();
		InitializeAttributes();

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		

		AddStartupEffects();

		AddCharacterAbilities();
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("AMage::PossessedBy: No PlayerState of Type AElementsPlayerState found for"));
	}
}

void AMage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMage::SetupPlayerInputComponent: PlayerController is not valid"));
	}
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMage::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMage::Look);
	for (const FInputAbilityMapping& Mapping : InputAbilityMappings)
	{
		if (Mapping.AbilityTags.IsValid() && Mapping.InputAction)
		{
			EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Started, this, &AMage::HandleAbilityInputPressed, Mapping.InputID);
			EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Completed, this, &AMage::HandleAbilityInputReleased, Mapping.InputID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMage::SetupPlayerInputComponent: AbilityTag or InputAction is not valid"))
		}
	}
	BindASCInputs();
	
}

void AMage::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AElementsPlayerState* PS = GetPlayerState<AElementsPlayerState>();
	if (PS)
	{
		hasASC = true;

		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UElementsAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		
		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInputs();


		AttributeSetBase = PS->GetAttributeSetBase();

		AElementsPlayerController* PC = Cast<AElementsPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();


		//// Respawn specific things that won't affect first possession.
		//
		//// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		//
		//// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMage::OnRep_PlayerState: No PlayerState of Type AElementsPlayerState found for"));
	}
}



void AMage::FinishDying()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AElementsGameMode* GM = Cast<AElementsGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->HeroDied(GetController());
		}
	}
	Super::FinishDying();
}

void AMage::Move(const FInputActionValue& Value)
{
	if (!IsAlive() || !CanMove()) 
	{
		return;
	}
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr) {
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

void AMage::Look(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMage::BeginJump()
{
	//TODO: replace with Jump Ability
	Jump();
}

void AMage::EndJump()
{
	//TODO: replace with Jump Ability
	StopJumping();
}

void AMage::HandleAbilityInputPressed(EAbilityInputID InputID)
{
	if (AbilitySystemComponent.IsValid()) 
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputID));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mage.HandleAbilityInputPressed: AbilitySystemComponent is not valid"));
	}
}

void AMage::HandleAbilityInputReleased(EAbilityInputID InputID)
{


	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(InputID));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Mage.HandleAbilityInputReleased: AbilitySystemComponent is not valid"));
	}
}

void AMage::BindASCInputs()
{
	if (!hasASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMage::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMage::Look);
		for (const FInputAbilityMapping& Mapping : InputAbilityMappings)
		{
			if (Mapping.AbilityTags.IsValid() && Mapping.InputAction)
			{
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Started, this, &AMage::HandleAbilityInputPressed, Mapping.InputID);
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Completed, this, &AMage::HandleAbilityInputReleased, Mapping.InputID);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMage::SetupPlayerInputComponent: AbilityTag or InputAction is not valid"))
			}
		}
		hasASCInputBound = true;
	}
	else
	{
		
		if (!AbilitySystemComponent.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("AMage::BindASCInputs: AbilitySystemComponent is not valid"));
		}
		if (IsValid(InputComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("AMage::BindASCInputs: InputComponent is not valid"));
		}
	}
}

//void AMage::HandleAbilityInputPressed(FGameplayTagContainer AbilityTag)
//{
//	if (AbilitySystemComponent.IsValid())
//	{
//		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTag);
//	}
//}
//
//void AMage::HandleAbilityInputReleased(FGameplayTagContainer AbilityTag)
//{
//	if (AbilitySystemComponent.IsValid()) 
//	{
//		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
//		AbilitySystemComponent->FindAllAbilitiesWithTags(AbilitySpecHandles, AbilityTag);
//	}
//}
