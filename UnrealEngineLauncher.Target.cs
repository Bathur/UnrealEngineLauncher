
using UnrealBuildTool;
using System.Collections.Generic;

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

		// Make sure to get all code in SlateEditorStyle compiled in
        bBuildDeveloperTools = true;

        // Compile out references from Core to the rest of the engine
        bCompileAgainstEngine = false;
        bCompileAgainstCoreUObject = true;

        // If ture the program entrance is WinMain, otherwise entrance is main
        bIsBuildingConsoleApplication = false;
    }
}

