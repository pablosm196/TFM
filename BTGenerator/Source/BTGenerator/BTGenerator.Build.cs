// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BTGenerator : ModuleRules
{
	public BTGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        PublicIncludePaths.AddRange(new string[] {
			"BTGenerator",
			"BTGenerator/Variant_Platforming",
			"BTGenerator/Variant_Platforming/Animation",
			"BTGenerator/Variant_Combat",
			"BTGenerator/Variant_Combat/AI",
			"BTGenerator/Variant_Combat/Animation",
			"BTGenerator/Variant_Combat/Gameplay",
			"BTGenerator/Variant_Combat/Interfaces",
			"BTGenerator/Variant_Combat/UI",
			"BTGenerator/Variant_SideScrolling",
			"BTGenerator/Variant_SideScrolling/AI",
			"BTGenerator/Variant_SideScrolling/Gameplay",
			"BTGenerator/Variant_SideScrolling/Interfaces",
			"BTGenerator/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
