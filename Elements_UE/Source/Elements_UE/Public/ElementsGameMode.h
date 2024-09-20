// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Characters/CharacterBase.h"
#include "ElementsGameMode.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AElementsGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AElementsGameMode();

	void HeroDied(AController* Character);

	void DropManaFromCharacterDeath(ACharacterBase* Character);

protected:
	float RespawnDelay;

	TSubclassOf<ACharacterBase> PlayerPawnClass;
	TSubclassOf<class AManaPickup> ManaPickupClass;


	void RespawnHero(AController* Controller);

	void SpawnManaPickup(ACharacterBase* Instigator, FVector Location, float ManaAmount);
};
