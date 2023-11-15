// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyPuzzlePlatform : ModuleRules
{
	public MyPuzzlePlatform(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "UMG"});
	}
}
