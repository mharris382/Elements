// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "AITargetedTokenContainer.generated.h"

USTRUCT(BlueprintType)
struct FTargetTokenDefinition 
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AITargetedToken", meta = (Categories = "AI.Tokens"))
	FGameplayTag TokenTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AITargetedToken")
	int MaxTokenCount;
};

struct ClaimedToken
{
	FGameplayTag TokenTag;
	TObjectPtr<AActor> Owner;
};

//USTRUCT(BlueprintType)
//struct FTargetTokenHandle
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AITargetedToken")
//	TObjectPtr<AActor> Owner;
//};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AICOORDINATION_API UAITargetedTokenContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAITargetedTokenContainer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	void ResetAllTokens();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AITargetedToken")
	TArray<FTargetTokenDefinition> TokenDefinitions;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UFUNCTION(BlueprintCallable, Category = "AI Tokens")
	int GetUnclaimedTokenCount(FGameplayTag TokenTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "AI Tokens")
	void UnclaimToken(FGameplayTag TokenTag, AActor* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "AI Tokens")
	bool ClaimToken(FGameplayTag TokenTag, AActor* Owner);

	UFUNCTION(BlueprintCallable, Category = "AI Tokens")
	bool HasToken(FGameplayTag TokenTag, AActor* Owner) const;

	UFUNCTION(BlueprintCallable, Category = "AI Tokens")
	bool StealToken(FGameplayTag TokenTag, AActor* Owner, AActor* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "AI Tokens")
	void UnlcaimAllTokens(AActor* Owner);

private:
	

	TMap<FGameplayTag, int> UnclaimedTokens;
	TMap<FGameplayTag, TArray<TObjectPtr<AActor>>> ClaimedTokens;
	
	bool bInitialized = false;

	void UnclaimFromDestroyedOwners();
};
