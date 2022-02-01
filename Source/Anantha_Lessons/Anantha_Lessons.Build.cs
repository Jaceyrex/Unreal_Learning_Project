// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Anantha_Lessons : ModuleRules
{
	public Anantha_Lessons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
