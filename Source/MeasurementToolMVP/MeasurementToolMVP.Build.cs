// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MeasurementToolMVP : ModuleRules
{
	public MeasurementToolMVP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(ModuleDirectory);
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"EnhancedInput",
			"InputCore",
			"Slate",
			"SlateCore",
			"UMG"
		});
	}
}
