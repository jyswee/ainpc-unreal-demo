using UnrealBuildTool;

public class AINPCDemoEditorTarget : TargetRules
{
	public AINPCDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("AINPCDemo");
	}
}
