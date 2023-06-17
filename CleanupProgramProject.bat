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

echo Try To Clean Up Old Project Files
del %UE_Engine_Dir%\Intermediate\ProjectFiles\%current_dir_name%.*
echo Old Project Files Is Cleaed
echo.
