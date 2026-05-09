using UnrealBuildTool;

public class AINPCDemo : ModuleRules
{
	public AINPCDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"Slate",
			"SlateCore",
			"AINPCEngine"
		});
	}
}
