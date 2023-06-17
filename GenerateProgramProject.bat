@echo off

set "UE_Engine_Dir=..\..\..\..\Engine"
echo Engine Directory Is [%UE_Engine_Dir%]
echo.

set "current_dir_name=%cd%"
:loop
set "current_dir_name=%current_dir_name:*\=%"
set "current_dir_nametmp=%current_dir_name:\=%"
if not "%current_dir_nametmp%"=="%current_dir_name%" goto loop
echo Current Directory Name Is [%current_dir_name%]
echo.

if exist "%UE_Engine_Dir%\Build\InstalledBuild.txt" (
  ren %UE_Engine_Dir%\Build\InstalledBuild.txt %UE_Engine_Dir%\Build\InstalledBuild_Backup.txt
  echo InstalledBuild.txt Found, Temporarily Rename It
  echo.
)

echo Try To Clean Up Old Project Files
del %UE_Engine_Dir%\Intermediate\ProjectFiles\%current_dir_name%.*
echo Old Project Files Is Cleaed
echo.

echo Using UnrealBuildTool Generate Program "%current_dir_name%" VisualStudio Solution
echo.
rem UE4.x
if exist "%UE_Engine_Dir%\Binaries\DotNET\UnrealBuildTool.exe" (
  cmd /c "%UE_Engine_Dir%\Binaries\DotNET\UnrealBuildTool.exe -notinstallengine -ProjectFiles %current_dir_name%"
)
rem UE5.x
if exist "%UE_Engine_Dir%\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
  cmd /c "%UE_Engine_Dir%\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -notinstallengine -ProjectFiles %current_dir_name%"
)

if exist "%UE_Engine_Dir%\Build\InstalledBuild_Backup.txt" (
  ren %UE_Engine_Dir%\Build\InstalledBuild_Backup.txt %UE_Engine_Dir%\Build\InstalledBuild.txt
  echo InstalledBuild.txt Restored
  echo.
)
