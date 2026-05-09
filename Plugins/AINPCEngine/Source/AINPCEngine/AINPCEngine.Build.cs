using UnrealBuildTool;

public class AINPCEngine : ModuleRules
{
    public AINPCEngine(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "HTTP",
            "WebSockets",
            "Json",
            "JsonUtilities"
        });
    }
}
