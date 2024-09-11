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
            "AIModule",
            "InputCore", 
			"EnhancedInput" ,
            "GameplayTags",
			"GameplayTasks",
            "GameplayAbilities",
			"Niagara"
        });
	}
}
