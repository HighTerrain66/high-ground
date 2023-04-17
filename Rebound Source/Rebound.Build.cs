// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rebound : ModuleRules
{
	public Rebound(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemSteam", "Niagara" });
	}
}
