// Copyright Epic Games, Inc. All Rights Reserved.

#include "Elements_UEGameMode.h"
#include "Elements_UECharacter.h"
#include "UObject/ConstructorHelpers.h"

AElements_UEGameMode::AElements_UEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Elements/Blueprints/BP_Mage"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
