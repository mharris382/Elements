// Fill out your copyright notice in the Description page of Project Settings.


#include "AITargetedTokenContainer.h"

// Sets default values for this component's properties
UAITargetedTokenContainer::UAITargetedTokenContainer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TokenDefinitions.Add(FTargetTokenDefinition{ FGameplayTag::RequestGameplayTag(FName("AI.Tokens.AoE")), 1 });
	TokenDefinitions.Add(FTargetTokenDefinition{ FGameplayTag::RequestGameplayTag(FName("AI.Tokens.Melee")), 3 });
	TokenDefinitions.Add(FTargetTokenDefinition{ FGameplayTag::RequestGameplayTag(FName("AI.Tokens.Projectile")), 3 });
	// ...
}

// Called when the game starts
void UAITargetedTokenContainer::BeginPlay()
{
	Super::BeginPlay();
	for (const FTargetTokenDefinition& TokenDef : TokenDefinitions)
	{
		ClaimedTokens.Add(TokenDef.TokenTag, TArray<TObjectPtr<AActor>>());
		UnclaimedTokens.Add(TokenDef.TokenTag, TokenDef.MaxTokenCount);
	}
}

void UAITargetedTokenContainer::ResetAllTokens()
{
	ClaimedTokens.Empty();
	UnclaimedTokens.Empty();
	for (const FTargetTokenDefinition& TokenDef : TokenDefinitions)
	{
		UnclaimedTokens.Add(TokenDef.TokenTag, TokenDef.MaxTokenCount);
	}
}

// Called every frame
void UAITargetedTokenContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UnclaimFromDestroyedOwners();
	// ...
}

int UAITargetedTokenContainer::GetUnclaimedTokenCount(FGameplayTag TokenTag) const
{
	if (UnclaimedTokens.Contains(TokenTag))
	{
		return UnclaimedTokens[TokenTag];
	}
	return 0;
}

void UAITargetedTokenContainer::UnclaimToken(FGameplayTag TokenTag, AActor* Owner)
{
	if (Owner && ClaimedTokens.Contains(TokenTag))
	{
		int unclaimed = ClaimedTokens[TokenTag].Remove(Owner);
		UnclaimedTokens[TokenTag] += unclaimed;
	}
}

bool UAITargetedTokenContainer::ClaimToken(FGameplayTag TokenTag, AActor* Owner)
{
	if (Owner && GetUnclaimedTokenCount(TokenTag) > 0 && !HasToken(TokenTag, Owner))
	{
		//only allow enemy to have one token claimed at a time, cannot perform 2 different actions at the same time
		UnlcaimAllTokens(Owner);
		UnclaimedTokens[TokenTag]--;
		ClaimedTokens[TokenTag].Add(Owner);
		return true;
	}
	return false;
}

bool UAITargetedTokenContainer::HasToken(FGameplayTag TokenTag, AActor* Owner) const
{
	if (Owner && ClaimedTokens.Contains(TokenTag))
	{
		return ClaimedTokens[TokenTag].Contains(Owner);
	}
	return false;
}

bool UAITargetedTokenContainer::StealToken(FGameplayTag TokenTag, AActor* Owner, AActor* NewOwner)
{
	if (Owner && NewOwner && HasToken(TokenTag, Owner) && !HasToken(TokenTag, NewOwner))
	{
		ClaimedTokens[TokenTag].Remove(Owner);
		ClaimedTokens[TokenTag].Add(NewOwner);
		return true;
	}
	return false;
}

void UAITargetedTokenContainer::UnlcaimAllTokens(AActor* Owner)
{
	if (Owner)
	{
		for (auto& Pair : ClaimedTokens)
		{
			TArray<TObjectPtr<AActor>>& Owners = Pair.Value;
			for (int i = Owners.Num() - 1; i >= 0; --i)
			{
				if (Owners[i] == Owner)
				{
					Owners.RemoveAt(i);
					UnclaimedTokens[Pair.Key]++;
				}
			}
		}
	}
}

void UAITargetedTokenContainer::UnclaimFromDestroyedOwners()
{
	for (auto& Pair : ClaimedTokens)
	{
		TArray<TObjectPtr<AActor>>& Owners = Pair.Value;
		for (int i = Owners.Num() - 1; i >= 0; --i)
		{
			if (!Owners[i])
			{
				Owners.RemoveAt(i);
				UnclaimedTokens[Pair.Key]++;
			}
		}
	}
}


