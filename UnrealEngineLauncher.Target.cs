using UnrealBuildTool;

[SupportedPlatforms(UnrealPlatformClass.All)]
public class UnrealEngineLauncherTarget : TargetRules
{
	public UnrealEngineLauncherTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Program;
		LinkType = TargetLinkType.Monolithic;

		LaunchModuleName = "UnrealEngineLauncher";
        if (bBuildEditor)
		{
			ExtraModuleNames.Add("EditorStyle");
		}

        bBuildDeveloperTools = true;

        bCompileAgainstEngine = false;
        bCompileAgainstCoreUObject = true;

        bIsBuildingConsoleApplication = false;
    }
}

