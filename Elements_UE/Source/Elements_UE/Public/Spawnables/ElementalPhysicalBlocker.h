// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawnables/ElementalActor.h"
#include "ElementalPhysicalBlocker.generated.h"


class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AElementalPhysicalBlocker : public AElementalActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MeshPivotComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

public:

	AElementalPhysicalBlocker();
};
