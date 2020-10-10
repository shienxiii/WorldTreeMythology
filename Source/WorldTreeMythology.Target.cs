// Copyright of Maple Game Studio

using UnrealBuildTool;
using System.Collections.Generic;

public class WorldTreeMythologyTarget : TargetRules
{
	public WorldTreeMythologyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "WorldTreeMythology" } );
	}
}
