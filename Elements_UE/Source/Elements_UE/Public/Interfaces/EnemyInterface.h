#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified. It acts as the Blueprint interface definition.
UINTERFACE(Blueprintable)
class ELEMENTS_UE_API UEnemyInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for enemies that can be implemented by both C++ and Blueprints.
 */
class ELEMENTS_UE_API IEnemyInterface
{
    GENERATED_BODY()

public:
    // Interface method to be implemented in Blueprint or C++ classes.
    
    // This function can be overridden in both C++ and Blueprint
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy")
    void PerformAttack();
    
    // Optionally, you can add more interface functions here
};