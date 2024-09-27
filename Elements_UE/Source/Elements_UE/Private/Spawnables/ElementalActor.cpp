// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawnables/ElementalActor.h"
#include "ElementSubsystem.h"
#include "ElementsBlueprintFunctionLibrary.h"

// Sets default values
AElementalActor::AElementalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FGameplayTag AElementalActor::GetElementTag()
{
	return ElementTag;
}

// Called when the game starts or when spawned
void AElementalActor::BeginPlay()
{
	Super::BeginPlay();
	FElementData ElementData;

	//check if instigator is a character or elemental actor (which potentially was part of a chain that started with a character)
	if (GetInstigator())
	{
		ACharacterBase* Character = Cast<ACharacterBase>(GetInstigator());
		//AElementalActor* ElementalActor = Cast<AElementalActor>(GetInstigator());
		if (Character) {
			OriginalInstigatorCharacter = Character;
		}
		/*else if(ElementalActor)
		{
			OriginalInstigatorCharacter = ElementalActor->OriginalInstigatorCharacter;
		}*/
	}
	GetElementListeners();
	if (ValidateElementTagOnBeginPlay(ElementData))
	{
		ApplyElementVisuals(ElementTag, ElementData.ElementColorID);
	}
	else 
	{
		//if (GEngine)
		//{
		//	FString msg = FString::Printf(TEXT("AElementalActor::BeginPlay: ElementTag is not valid for ElementalActor %s"), *GetNameSafe(this));
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, msg);
		//}
		UE_LOG(LogTemp, Error, TEXT("AElementalActor::BeginPlay: ElementTag is not valid for ElementalActor %s"), *GetNameSafe(this))
	}
}

void AElementalActor::UpdateElementVisuals_Implementation(FGameplayTag InElementTag, FLinearColor ElementData)
{
}

// Called every frame
void AElementalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElementalActor::ApplyElementVisuals(FGameplayTag InElementTag, FLinearColor ElementColor)
{
	UpdateElementVisuals(ElementTag, ElementColor);
	for (UObject* Listener : ElementListeners)
	{
		IElementListenerInterface* ElementListener = Cast<IElementListenerInterface>(Listener);
		ElementListener->Execute_SetElement(Listener, InElementTag, ElementColor);
	}
}

bool AElementalActor::ValidateElementTagOnBeginPlay(FElementData& OutElementData)
{
	if (ElementTag.IsValid())
	{
		if (UElementsBlueprintFunctionLibrary::GetElementDataFromTag(this, ElementTag, OutElementData))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AElementalActor::BeginPlay: ElementTag %s is not valid %s"), *ElementTag.ToString(), *GetNameSafe(this))
		}
	}
	else if (AttemptToResolveElementTagFromInstigator(OutElementData))
	{
		return true;
	}
	return false;
}

bool AElementalActor::AttemptToResolveElementTagFromInstigator(FElementData& OutElementData)
{
	if (!GetInstigator())
	{
		return false;
	}
	if (GetInstigator()->Implements<UElementInterface>())
	{
		IElementInterface* ElementInterface = Cast<IElementInterface>(GetInstigator());
		if (ElementInterface)
		{
			ElementTag = ElementInterface->Execute_GetElementTag(GetInstigator());
			if (UElementsBlueprintFunctionLibrary::GetElementDataFromTag(this, ElementTag, OutElementData))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
	}
	return false;
}

void AElementalActor::GetElementListeners()
{
	ElementListeners.Empty();
	TArray<UActorComponent*> Components = GetComponentsByInterface(UElementListenerInterface::StaticClass());
	for (UActorComponent* Component : Components)
	{
		
		ElementListeners.Add(Component);
		
	}
}

