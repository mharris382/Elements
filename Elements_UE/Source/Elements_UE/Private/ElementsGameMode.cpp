// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementsGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
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
