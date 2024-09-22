// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Elements_UE : ModuleRules
{
	public Elements_UE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine",
            "PhysicsCore",
            "AIModule",
            "InputCore", 
			"EnhancedInput" ,
            "GameplayTags",
			"GameplayTasks",
            "GameplayAbilities",
			"Niagara",
            "DataRegistry"
        });


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                //"Slate",
                //"SlateCore",
                //"RenderCore",
                //"DeveloperSettings",
                //"NetCore",
                //"RHI",
                //"Projects",
                //"Gauntlet",
                "UMG",
                "CommonUI",
                "CommonInput",
                //"GameSettings",
                //"CommonGame",
                //"CommonUser",
                //"GameSubtitles",
                //"GameplayMessageRuntime",
                //"AudioMixer",
                //"NetworkReplayStreaming",
                //"UIExtension",
                //"ClientPilot",
                //"AudioModulation",
                //"EngineSettings",
                //"DTLSHandlerComponent",
            }
        );
    }
}
