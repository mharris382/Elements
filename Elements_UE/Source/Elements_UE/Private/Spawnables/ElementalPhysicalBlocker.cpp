// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawnables/ElementalPhysicalBlocker.h"
#include "Components/StaticMeshComponent.h"


AElementalPhysicalBlocker::AElementalPhysicalBlocker()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.1f;
	bReplicates = true;
	SetReplicateMovement(true);

	MeshPivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	RootComponent = MeshPivotComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}
