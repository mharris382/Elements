// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ElementsActivatableWidget.h"

UElementsActivatableWidget::UElementsActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UElementsActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EElementsWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EElementsWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EElementsWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EElementsWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
