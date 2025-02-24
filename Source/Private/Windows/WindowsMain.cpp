#include "RealExecutionMain.h"
#include <windows.h>
#include <shellapi.h>

int WinMain(
	_In_ HINSTANCE hInInstance,
	_In_opt_ HINSTANCE hInPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	hInstance = hInInstance;
	
	int32 ErrorLevel = 0;
	ErrorLevel = RealExecutionMain(GetCommandLineW());

	FPlatformMisc::RequestExit(true);
	FEngineLoop::AppExit();
	
	return ErrorLevel;
}
