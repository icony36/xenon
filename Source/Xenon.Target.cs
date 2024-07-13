// Copyright CoNY. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class XenonTarget : TargetRules
{
	public XenonTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Xenon");
	}
}
