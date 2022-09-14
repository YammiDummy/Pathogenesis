// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pathogenesis : ModuleRules
{
	public Pathogenesis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"HeadMountedDisplay", 
				"NavigationSystem", 
				"AIModule",
                "Slate",
                "SlateCore",
                "UMG",
				"PhysicsCore",

            });
    }
}
