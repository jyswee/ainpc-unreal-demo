using UnrealBuildTool;

public class AINPCDemoTarget : TargetRules
{
	public AINPCDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("AINPCDemo");
	}
}
