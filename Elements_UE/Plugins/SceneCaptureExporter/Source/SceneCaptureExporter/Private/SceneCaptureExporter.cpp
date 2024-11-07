// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneCaptureExporter.h"
#include "ImageUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "HighResScreenshot.h"
//#include "ImageWriteBlueprintLibrary.h"
#include "Components/SceneCaptureComponent2D.h"

#define LOCTEXT_NAMESPACE "FSceneCaptureExporterModule"

//void CaptureAndSaveScene(UWorld* World, const FString& FilePath)
//{
//    if (!World) return;
//
//    // Create a Scene Capture 2D Actor
//    ASceneCapture2D* SceneCapture = World->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass());
//    if (!SceneCapture) return;
//
//    // Set up the render target
//    UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
//    RenderTarget->InitAutoFormat(1920, 1080); // Set desired resolution
//    RenderTarget->ClearColor = FLinearColor::Black;
//    SceneCapture->GetCaptureComponent2D()->TextureTarget = RenderTarget;
//    SceneCapture->GetCaptureComponent2D()->CaptureScene();
//
//    // Read the pixels from the RenderTarget
//    FRenderTarget* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
//    TArray<FColor> Bitmap;
//    RenderTargetResource->ReadPixels(Bitmap);
//
//    // Save the image using the ImageWriteQueue
//    FImageWriteOptions Options;
//    Options.Format = EImageFormat::PNG;
//    Options.CompressionQuality = 100;
//    Options.bAsync = false;
//    UImageWriteBlueprintLibrary::ExportToDisk(RenderTarget, FilePath, Options);
//
//    // Clean up
//    SceneCapture->Destroy();
//    RenderTarget->RemoveFromRoot();


void FSceneCaptureExporterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSceneCaptureExporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSceneCaptureExporterModule, SceneCaptureExporter)