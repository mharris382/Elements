// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ElementsPlayerController.h"
#include "Player/ElementsPlayerState.h"
#include "Characters/Mage.h"
#include "AbilitySystemComponent.h"

void AElementsPlayerController::CreateHUD()
{
	//TODO: Create HUD
}

void AElementsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AElementsPlayerState* PS = GetPlayerState<AElementsPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AElementsPlayerController::OnPossess: PlayerState is null"));
	}
}

void AElementsPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}
