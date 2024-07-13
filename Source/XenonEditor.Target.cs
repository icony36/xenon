// Copyright CoNY. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class XenonEditorTarget : TargetRules
{
	public XenonEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Xenon");
	}
}
