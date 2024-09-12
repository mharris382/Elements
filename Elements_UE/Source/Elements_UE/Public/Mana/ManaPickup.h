// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "ElementSubsystem.h"
#include "ManaPickup.generated.h"

UCLASS()
class ELEMENTS_UE_API AManaPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManaPickup();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	FGameplayTag ElementTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Element")
	void SetElementType(FGameplayTag NewElementTag);

	UFUNCTION(BlueprintImplementableEvent, Category = "Element")
	void UpdateElementVisuals(FElementData ElementData);
};
