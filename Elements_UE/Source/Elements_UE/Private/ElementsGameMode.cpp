// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementsGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Mana/ManaPickup.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/ElementsPlayerController.h"
#include "Player/ElementsPlayerState.h"
#include "Characters/Mage.h"

AElementsGameMode::AElementsGameMode()
{
	RespawnDelay = 5.0f;

	PlayerPawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Elements/Blueprints/BP_Mage.BP_Mage_C"));
	if (!PlayerPawnClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find HeroClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
	ManaPickupClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Elements/Blueprints/BP_ManaPickup.BP_ManaPickup_C"));
	if (!ManaPickupClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find ManaPickupClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}
}

void AElementsGameMode::HeroDied(AController* Controller)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);
	
	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);
	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;
	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GameMode: Respawning Hero in 5 seconds"));
	}
	RespawnDelegate = FTimerDelegate::CreateUObject(this, &AElementsGameMode::RespawnHero, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);


	//TODO: Show some kind of respawn countdown on the player's screen
	/*AGDPlayerController* PC = Cast<AGDPlayerController>(Controller);
	if (PC)
	{
		PC->SetRespawnCountdown(RespawnDelay);
	}*/
}

void AElementsGameMode::DropManaFromCharacterDeath(ACharacterBase* Character)
{
	if (!Character) 
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode DropManaFromCharacterDeath: Character is null"))
		return;
	}
	
	UAttributeSetBase* AttributeSet = Character->GetAttributeSetBase();
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode DropManaFromCharacterDeath: AttributeSet is null"))
			return;
	}

	if (!ManaPickupClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode DropManaFromCharacterDeath: ManaPickupClass is not set"))
			return;
	}

	float AvailableMana  = AttributeSet->GetMana();
	if(AvailableMana <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("GameMode DropManaFromCharacterDeath: Character %s has no mana to drop"), )
		return;
	}
	AManaPickup* DefaultManaPickup = Cast<AManaPickup>(ManaPickupClass->GetDefaultObject());
	float ManaPerPickup = DefaultManaPickup->ManaAmount;

	if (AvailableMana < ManaPerPickup) 
	{
		SpawnManaPickup(Character, Character->GetActorLocation(), ManaPerPickup);
	}
	else
	{
		int NumberOfDrops = FMath::Min(FMath::FloorToInt(AvailableMana / ManaPerPickup), 5);
		for (size_t i = 0; i < NumberOfDrops; i++)
		{
			SpawnManaPickup(Character, Character->GetActorLocation(), ManaPerPickup);
		}
	}
}

void AElementsGameMode::RespawnHero(AController* Controller)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("GameMode: Respawning Hero Now"));
	}
	if (!Controller) {
		UE_LOG(LogTemp, Error, TEXT("GameMode RespawnHero: Failed to Respawn Hero because Controller is null"))
		return;
	}
	if (Controller->IsPlayerController())
	{
		AActor* PlayerStart = FindPlayerStart(Controller);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ACharacterBase* Mage = GetWorld()->SpawnActor<ACharacterBase>(PlayerPawnClass, PlayerStart->GetActorTransform(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		if (OldSpectatorPawn)
		{
			OldSpectatorPawn->Destroy();
		}
		Controller->Possess(Mage);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("GameMode RespawnHero: Failed to Respawn Hero because Controller is not a PlayerController. Controller Type: %s"), *Controller->GetClass()->GetName());
	}
}

void AElementsGameMode::SpawnManaPickup(ACharacterBase* InInstigator, FVector Location, float ManaAmount)
{
	FVector RandomOffset = FVector(FMath::RandRange(-50.0f, 50.0f), FMath::RandRange(-50.0f, 50.0f), 0.0f);
	FVector SpawnLocation = Location + RandomOffset;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FGameplayTag ElementTag = InInstigator->GetElementTag();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.Instigator = InInstigator;
	AManaPickup* ManaPickup = GetWorld()->SpawnActor<AManaPickup>(ManaPickupClass, SpawnLocation, SpawnRotation, SpawnParameters);
	ManaPickup->SetElementType(ElementTag);
	ManaPickup->SetManaAmount(ManaAmount);
}
