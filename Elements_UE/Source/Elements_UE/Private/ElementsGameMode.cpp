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

	PlayerPawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Elements/Blueprints/BP_Mage"));

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

	//FTimerDelegate::CreateUObject(this, &AElementsGameMode::RespawnHero, Controller);
}

void AElementsGameMode::RespawnHero(AController* Controller)
{


}
