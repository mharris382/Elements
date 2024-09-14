// Fill out your copyright notice in the Description page of Project Settings.



#include "Characters/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer)
{
	HardRefAbilitySystemComponent = CreateDefaultSubobject<UElementsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);

	HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = HardRefAbilitySystemComponent;

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	HardRefAttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	// Set our parent's TWeakObjectPtr
	AttributeSetBase = HardRefAttributeSetBase;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	//TODO: Health Bar Stuff
	/*UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIFloatingStatusBarComponent"));
	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500, 500));*/
	
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Begin Play"));
	}

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		if (AttributeSetBase->GetMaxHealth() == 0 && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Initialized Attributes but Max Health is still ZERO!!! :( "));
		}
		AddStartupEffects();
		AddCharacterAbilities();
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Initialized Enemy Abilities"));
		}
		UE_LOG(LogTemp, Error, TEXT("Initialized Enemy Abilities"));


		
		//UI STUFF WOULD GO HERE

		// Setup FloatingStatusBar UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
		//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//if (PC && PC->IsLocalPlayerController())
		//{
		//	if (UIFloatingStatusBarClass)
		//	{
		//		UIFloatingStatusBar = CreateWidget<UGDFloatingStatusBarWidget>(PC, UIFloatingStatusBarClass);
		//		if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
		//		{
		//			UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);

		//			// Setup the floating status bar
		//			UIFloatingStatusBar->SetHealthPercentage(GetHealth() / GetMaxHealth());

		//			UIFloatingStatusBar->SetCharacterName(CharacterName);
		//		}
		//	}
		//}
		
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AEnemyCharacter::HealthChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::StunTagChanged);
	}
	else 
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to validateAbility system component"));
		}
		UE_LOG(LogTemp, Error, TEXT("Failed to find valid Ability System Component on EnemyCharacter"));
	}
}

void AEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		Die();
	}
	UE_LOG(LogTemp, Warning, TEXT("HealthChanged: %f"), Data.NewValue);
}

void AEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));


		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
	UE_LOG(LogTemp, Warning, TEXT("StunTagChanged: %d"), NewCount);
}
