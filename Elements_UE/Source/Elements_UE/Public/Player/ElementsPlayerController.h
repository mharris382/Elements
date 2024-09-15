// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ElementsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AElementsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void CreateHUD();

protected:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
