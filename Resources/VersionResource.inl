// Copyright Epic Games, Inc. All Rights Reserved.

#ifndef APSTUDIO_INVOKED

#include <windows.h>
#include "../../../Runtime/Launch/Resources/Windows/resource.h"
#include "../../../Runtime/Launch/Resources/Version.h"

/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION ENGINE_MAJOR_VERSION,ENGINE_MINOR_VERSION,ENGINE_PATCH_VERSION,0
 PRODUCTVERSION ENGINE_MAJOR_VERSION,ENGINE_MINOR_VERSION,ENGINE_PATCH_VERSION,0
 FILEFLAGSMASK 0x17L
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x4L
 FILETYPE 0x2L
 FILESUBTYPE 0x0L
BEGIN
	BLOCK "StringFileInfo"
	BEGIN
		BLOCK "040904b0"
		BEGIN
			VALUE "CompanyName", "bathur.cn"
			VALUE "LegalCopyright", EPIC_COPYRIGHT_STRING
			VALUE "ProductName", "Unreal Engine Launcher"
			VALUE "ProductVersion", ENGINE_VERSION_STRING
			VALUE "FileDescription", "Unreal Engine Launcher"
			VALUE "InternalName", EPIC_PRODUCT_IDENTIFIER
			VALUE "OriginalFilename", "UnrealEngineLauncher.exe"
			VALUE "ShellIntegrationVersion", "2"
		END
	END
	BLOCK "VarFileInfo"
	BEGIN
		VALUE "Translation", 0x409, 1200
	END
END

#endif

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDICON_UEGame			ICON                    "Icon.ico"