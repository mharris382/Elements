// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ElementInterface.h"
#include "Interfaces/ElementListenerInterface.h"
#include "Characters/CharacterBase.h"
#include "GameplayTagContainer.h"
#include "ElementalActor.generated.h"

UCLASS()
class ELEMENTS_UE_API AElementalActor : public AActor, public IElementInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElementalActor();

	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Element")
	FGameplayTag ElementTag;

	FGameplayTag GetElementTag();

	//this will happen automatically if element is not specified
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Element")
	//bool bInheritElementFromInstigator = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Element")
	ACharacterBase* OriginalInstigatorCharacter = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;





	UFUNCTION(BlueprintNativeEvent, Category = "Element")
	void UpdateElementVisuals(FGameplayTag InElementTag, FLinearColor ElementColor);


	void UpdateElementVisuals_Implementation(FGameplayTag InElementTag, FLinearColor ElementColor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	

private:

	void ApplyElementVisuals(FGameplayTag InElementTag, FLinearColor ElementColor);

	bool ValidateElementTagOnBeginPlay(struct FElementData& OutElementData);
	bool AttemptToResolveElementTagFromInstigator(FElementData& OutElementData);


	void GetElementListeners();

	TArray<UObject*> ElementListeners;
};
