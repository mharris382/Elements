// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ElementsActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class EElementsWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};
/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API UElementsActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UElementsActivatableWidget(const FObjectInitializer& ObjectInitializer);


	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EElementsWidgetInputMode InputConfig = EElementsWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
