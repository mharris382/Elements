// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInterface.h"
#include "CaptureExporter.generated.h"

// Struct to hold capture settings such as resolution, projection type, and FOV
USTRUCT(BlueprintType)
struct FRenderCaptureSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Settings", meta = (ToolTip = "Resolution for the capture output (Width x Height)."))
    FIntPoint Resolution = FIntPoint(1920, 1080);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Settings", meta = (EditCondition = "!bUseOrthographicProjection", ToolTip = "Field of View (FOV) for the capture when using perspective projection."))
    float FieldOfView = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Settings", meta = (EditCondition = "bUseOrthographicProjection", ToolTip = "Width of the orthographic view. Controls the zoom level when using orthographic projection."))
    float OrthographicWidth = 512.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Settings", meta = (ToolTip = "Enable orthographic projection if true; otherwise, use perspective projection."))
    bool bUseOrthographicProjection = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Settings", meta = (ToolTip = "If enabled, the capture will only include specified actors in the Capture Targets list."))
    bool bOnlyShowSpecifiedActors = false;

    FRenderCaptureSettings()
        : Resolution(1920, 1080), FieldOfView(90.0f), OrthographicWidth(512.0f), bUseOrthographicProjection(true), bOnlyShowSpecifiedActors(false)
    {
    }
};

// Struct to hold settings for pivot-based alignment
USTRUCT(BlueprintType)
struct FPivotSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pivot Settings", meta = (ToolTip = "Actor to use as a pivot for aligning the capture component's position and orientation."))
    AActor* PivotActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pivot Settings", meta = (ToolTip = "Axis for orienting the capture component towards the pivot actor."))
    FRotator PivotAlignmentAxis = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pivot Settings", meta = (ToolTip = "Offset distance along the alignment axis. If negative, the scene capture will face the pivot directly."))
    float Offset = 100.0f;
};


// Struct to hold settings for each capture pass, such as the pass name, post-process material, and render target
USTRUCT(BlueprintType)
struct FSceneCapturePass
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Capture Pass", meta = (ToolTip = "Unique name identifier for this capture pass."))
    FName PassName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Capture Pass", meta = (ToolTip = "Post-process material applied during the capture to achieve effects like bloom or color grading."))
    UMaterialInterface* PassPostProcessCaptureMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Capture Pass", meta = (ToolTip = "Render target used to store the output of this capture pass."))
    UTextureRenderTarget2D* PassRenderTarget = nullptr;
};

UCLASS()
class SCENECAPTUREEXPORTER_API ACaptureExporter : public AActor
{
    GENERATED_BODY()

public:
    ACaptureExporter();

    // Function to capture the scene, accessible via an editor button
    UFUNCTION(CallInEditor, Category = "Scene Capture", meta = (ToolTip = "Capture the scene to the specified render target."))
    void Capture();

    // Separate export function (to be implemented separately)
    void Export();

    // Scene capture component for capturing the scene
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USceneCaptureComponent2D* SceneCaptureComponent;

    //// Main function to capture and export the scene, accessible via an editor button
    //UFUNCTION(CallInEditor, Category = "Scene Capture", meta = (ToolTip = "Capture the scene and export to disk using the specified settings."))
    //void CaptureAndExport();

#if WITH_EDITOR
    // Called when a property is changed in the editor
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
    // Updates the capture component settings dynamically based on user preferences
    void UpdateCaptureComponentSettings();

    // Aligns and positions the SceneCaptureComponent based on PivotSettings
    void ApplyPivotSettings();

private:
    // Core capture settings (resolution, projection type, FOV, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Configuration", meta = (ToolTip = "Settings for render capture, including resolution, projection type, and optional target actors."), meta = (AllowPrivateAccess = "true"))
    FRenderCaptureSettings CaptureSettings;

    // List of actors to exclusively render during capture
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Configuration", meta = (ToolTip = "Array of actors to exclusively render during capture if the Only Show Specified Actors option is enabled."), meta = (AllowPrivateAccess = "true"))
    TArray<AActor*> CaptureTargets;

    // Settings for pivot-based alignment
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Configuration", meta = (ToolTip = "Settings for aligning the capture component based on a pivot actor."), meta = (AllowPrivateAccess = "true"))
    FPivotSettings PivotSettings;

    // Array of capture passes for applying post-process materials and storing different render targets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Passes", meta = (ToolTip = "Array of capture passes to apply during the capture, each with unique settings like post-process material and render target."), meta = (AllowPrivateAccess = "true"))
    TArray<FSceneCapturePass> CapturePasses;
};
