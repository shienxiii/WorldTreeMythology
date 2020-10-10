// Copyright of Maple Game Studio

using UnrealBuildTool;
using System.Collections.Generic;

public class WorldTreeMythologyEditorTarget : TargetRules
{
	public WorldTreeMythologyEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "WorldTreeMythology" } );
	}
}
