// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LestaAcademy : ModuleRules
{
	public LestaAcademy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Paper2D", "UMG" });
	}
}
