// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureExporter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"

ACaptureExporter::ACaptureExporter()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    SceneCaptureComponent->SetupAttachment(RootComponent);

    // Apply initial settings to the capture component
    UpdateCaptureComponentSettings();
}

void ACaptureExporter::Capture()
{
    if (!SceneCaptureComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("SceneCaptureComponent is not initialized."));
        return;
    }

    // Loop through each capture pass
    for (const FSceneCapturePass& Pass : CapturePasses)
    {
        if (!Pass.PassRenderTarget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Render target for pass '%s' is not set. Skipping this pass."), *Pass.PassName.ToString());
            continue;
        }

        // Apply the post-process material and render target for the current pass
        SceneCaptureComponent->PostProcessSettings.RemoveBlendable(0);  // Clear any existing blendable
        if (Pass.PassPostProcessCaptureMaterial)
        {
            SceneCaptureComponent->PostProcessSettings.AddBlendable(Pass.PassPostProcessCaptureMaterial, 1.0f);
        }

        SceneCaptureComponent->TextureTarget = Pass.PassRenderTarget;

        // Capture the scene with the current pass's settings
        SceneCaptureComponent->CaptureScene();

        UE_LOG(LogTemp, Log, TEXT("Scene captured for pass '%s'."), *Pass.PassName.ToString());
    }

    // Clear the render target and post-process material after capturing all passes
    SceneCaptureComponent->TextureTarget = nullptr;
    SceneCaptureComponent->PostProcessSettings.RemoveBlendable(0);
}

void ACaptureExporter::Export()
{
}

// Applies all settings from FRenderCaptureSettings to the SceneCaptureComponent
void ACaptureExporter::UpdateCaptureComponentSettings()
{
    if (!SceneCaptureComponent) return;

    // Set projection type and FOV or orthographic width based on CaptureSettings
    if (CaptureSettings.bUseOrthographicProjection)
    {
        SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
        SceneCaptureComponent->OrthoWidth = CaptureSettings.OrthographicWidth;
    }
    else
    {
        SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Perspective;
        SceneCaptureComponent->FOVAngle = CaptureSettings.FieldOfView;
    }

    // Configure actors to capture (ShowOnlyActors) if specified
    SceneCaptureComponent->ShowOnlyActors.Empty();
    if (CaptureSettings.bOnlyShowSpecifiedActors && CaptureTargets.Num() > 0)
    {
        for (AActor* Target : CaptureTargets)
        {
            if (Target)
            {
                SceneCaptureComponent->ShowOnlyActors.Add(Target);
            }
        }
    }
}

// Aligns and positions the SceneCaptureComponent based on FPivotSettings
void ACaptureExporter::ApplyPivotSettings()
{
    if (!PivotSettings.PivotActor || !SceneCaptureComponent) return;

    // Get the pivot location
    FVector PivotLocation = PivotSettings.PivotActor->GetActorLocation();

    // Calculate the forward direction based on the alignment axis
    FVector ForwardDirection = PivotSettings.PivotAlignmentAxis.Vector();

    // Calculate the position with offset along the desired axis
    FVector OffsetLocation = PivotLocation + (ForwardDirection * PivotSettings.Offset);

    // Set the location of the scene capture component
    SetActorLocation(OffsetLocation);

    // If Offset is negative, rotate to face the pivot
    if (PivotSettings.Offset < 0.0f)
    {
        FRotator LookAtRotation = (PivotLocation - OffsetLocation).Rotation();
        SetActorRotation(LookAtRotation);
    }
    else
    {
        // Align with the specified alignment axis
        SetActorRotation(PivotSettings.PivotAlignmentAxis);
    }
}

#if WITH_EDITOR
// Called when a property is changed in the editor
void ACaptureExporter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Reapply settings when properties change in the editor
    UpdateCaptureComponentSettings();
    ApplyPivotSettings();
}
#endif

