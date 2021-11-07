@echo off
set PROJECT_NAME=UE4Launcher
md %PROJECT_NAME%\Engine\Binaries\Win64
md %PROJECT_NAME%\Engine\Content\Internationalization\icudt64l
md %PROJECT_NAME%\Engine\Content\Slate 
md %PROJECT_NAME%\Engine\Shaders\StandaloneRenderer
copy ..\..\..\Binaries\Win64\%PROJECT_NAME%*.exe %PROJECT_NAME%\Engine\Binaries\Win64
copy Register.reg %PROJECT_NAME%\
copy UnRegister.reg %PROJECT_NAME%\
xcopy /y/i/s/e ..\..\..\Content\Internationalization\English\icudt64l %PROJECT_NAME%\Engine\Content\Internationalization\icudt64l
xcopy /y/i/s/e ..\..\..\Content\Slate %PROJECT_NAME%\Engine\Content\Slate
xcopy /y/i/s/e ..\..\..\Shaders\StandaloneRenderer %PROJECT_NAME%\Engine\Shaders\StandaloneRenderer
copy Resources\icon.ico %PROJECT_NAME%\Engine\Content\Slate\Icons
echo "Engine\Binaries\Win64\%PROJECT_NAME%.exe">%PROJECT_NAME%\%PROJECT_NAME%.bat

