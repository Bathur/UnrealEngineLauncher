
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

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
                "Runtime/Launch/Private",  // For LaunchEngineLoop.cpp include
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
			"CURRENT_VERSION_ID=24",
			"REMOTE_VERSION_FILE=\"\""
		});
	}
}

