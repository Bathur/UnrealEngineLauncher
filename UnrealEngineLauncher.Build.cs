using UnrealBuildTool;

public class UnrealEngineLauncher : ModuleRules
{
	public UnrealEngineLauncher(ReadOnlyTargetRules Target) : base(Target)
	{
        PublicIncludePaths.AddRange(
            new string[]
            {
                "Runtime/Launch/Public",
                "Programs/UnrealEngineLauncher/Source/Public",
				"Runtime/Core/Public/Containers",
                "Runtime/Core/Public/Misc"
            }); 
        PrivateIncludePaths.AddRange(
            new string[]
            {
				// For LaunchEngineLoop.cpp Include
                "Runtime/Launch/Private",  
                "Programs/UnrealEngineLauncher/Source/Private"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "AppFramework",
                "Core",
                "ApplicationCore",
                "Projects",
                "DesktopPlatform",
                "Slate",
                "SlateCore",
                "InputCore",
                "SlateReflector",
                "StandaloneRenderer",
                "WebBrowser",
                "SourceCodeAccess",
				"OpenGL",
				"HTTP"
            }
        );
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "SlateReflector",
            }
        );
        PublicIncludePathModuleNames.AddRange(
            new string[] {
                "SlateReflector",
            }
        );
		PublicDefinitions.AddRange(new string[]
		{
			"TOOL_NAME=\"UnrealEngineLauncher\"",
			"CURRENT_VERSION_ID=25",
			"REMOTE_VERSION_FILE=\"\""
		});
	}
}

