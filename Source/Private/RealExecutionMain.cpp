
#include "RealExecutionMain.h"
#include "LaunchEngineLoop.h"
#include "StandaloneRenderer.h"
#include "Framework/Application/SlateApplication.h"
#include "DesktopPlatformModule.h"
#include "ISlateReflectorModule.h"
#include "SWebBrowser.h"
#include "IWebBrowserWindow.h"
#include "IWebBrowserPopupFeatures.h"
#include <iostream>

// project files
#include "SlateWidget/SConfPanel.h"
#include "Data/FLaunchConf.h"
#include "Tools/EngineLaunchTools.h"
#include "Tools/SerializationTools.h"
#include "Tools/CommandLineParase.h"
#include <cctype>

#define LOCTEXT_NAMESPACE "UnrealEngineLauncher"

IMPLEMENT_APPLICATION(UnrealEngineLauncher, "UnrealEngineLauncher");
namespace CommandHandler{
	// Edit Mode
	void CreateConfWindowByLaunchParams(const FString& Param);
	// Launch Config don't create Config Panel
	void LaunchConfInstanceByCmdLine(const FString& Param);
	// from .uproject create .uejson
	void GenerateLaunchConfFileByUproject(const FString& Param);


	void HandleMainWindowClosed(const TSharedRef<SWindow>& pMainWindow);

	bool CmdKeyHasMatched(const TArray<FString>& pTokens,const TArray<FString>& pKeys);
	static TSharedPtr<SWindow> MainWindow;
	static bool HasWindow=false;
};


namespace WindowManager
{
	TSharedPtr<SWindow> CreateConfWindow(const FLaunchConf& Conf, const FString& ConfFile = TEXT(""));

	void OnOpenFileChangeWindowTitle(const FString& File);

	static TArray<TSharedPtr<SWindow>> WindowsList;
};



int RealExecutionMain(const TCHAR* pCmdLine)
{
	// add .uejson file mapping for administrator user
	EngineLaunchTools::UnrealEngineLauncherRegisterWriter();

	FCommandLine::Set(pCmdLine);
	// start up the main loop
	GEngineLoop.PreInit(pCmdLine);
	// Make sure all UObject classes are registered and default properties have been initialized
	ProcessNewlyLoadedUObjects();
	// Tell the module manager it may now process newly-loaded UObjects when new C++ modules are loaded
	FModuleManager::Get().StartProcessingNewlyLoadedObjects();
	// crank up a normal Slate application using the platform's standalone renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());
	// set the application name
	FGlobalTabmanager::Get()->SetApplicationTitle(LOCTEXT("AppTitle", "UE Launcher"));
	FModuleManager::LoadModuleChecked<ISlateReflectorModule>("SlateReflector").RegisterTabSpawner(FWorkspaceItem::NewGroup(LOCTEXT("DeveloperMenu", "Developer")));


	TMap<FString, void(*)(const FString&)> ParamsHandlerMaps;
	TArray<FString> AllParamsHandlerKey;
	ParamsHandlerMaps.Add(TEXT("e"), &CommandHandler::CreateConfWindowByLaunchParams);
	ParamsHandlerMaps.Add(TEXT("c"), &CommandHandler::LaunchConfInstanceByCmdLine);
	ParamsHandlerMaps.Add(TEXT("g"), &CommandHandler::GenerateLaunchConfFileByUproject);
	ParamsHandlerMaps.GetKeys(AllParamsHandlerKey);

	TMap<FString, FString> CommandArgsMaps = CommandLineParase::GetCommandLineParamsMap(FCommandLine::Get());
	TArray<FString> AllParamsKeys;
	CommandArgsMaps.GetKeys(AllParamsKeys);

	if (CommandHandler::CmdKeyHasMatched(AllParamsHandlerKey, AllParamsKeys))
	{
		for (const auto& ParamItem : AllParamsKeys)
		{
			if (ParamsHandlerMaps.Contains(ParamItem))
			{
				void(*Handler)(const FString&) = *ParamsHandlerMaps.Find(ParamItem);
				if (Handler)
				{
					Handler(*CommandArgsMaps.Find(ParamItem));
					break;
				}
			}
		}
	}
	else
	{
		CommandHandler::CreateConfWindowByLaunchParams(TEXT(""));
	}

	if (CommandHandler::HasWindow)
	{
		// main loop
		while (!IsEngineExitRequested())
		{
			FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
			FStats::AdvanceFrame(false);
			FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
			FSlateApplication::Get().PumpMessages();
			FSlateApplication::Get().Tick();
			FPlatformProcess::Sleep(0.03);
		}
	}

	FModuleManager::Get().UnloadModulesAtShutdown();
	FSlateApplication::Shutdown();

	return 0;
}


#include "SlateWidget/SConfigListPanel.h"
#include "SlateWidget/SVersionUpdater/SVersionUpdaterWidget.h"
namespace WindowManager
{
	static TSharedPtr < SConfPanel > LauncherPanel;
	static TSharedPtr < SConfigListPanel > LauncherConfListPanel;
	static TSharedPtr < SVersionUpdaterWidget > VersionUpdaterWidget;
	void OnConfigSelected(FLaunchConf Config)
	{
		if(LauncherPanel.IsValid())
		{
			LauncherPanel->UpdateAll(Config);
		}
	}
	void OnAddToGlobal(FLaunchConf Config)
	{
		if(LauncherConfListPanel.IsValid())
		{
			LauncherConfListPanel->AddConfig(Config);
		}
	}
	TSharedPtr<SWindow> CreateConfWindow(const FLaunchConf& Conf, const FString& ConfFile)
	{
		TSharedPtr<SWindow> ConfWindow = SNew(SWindow)
			.Title(LOCTEXT("MainWindow_Title", "UE Launcher"))
			.ScreenPosition(FVector2D(800, 1200))
			.ClientSize(FVector2D(1200, 800))
			.SupportsMaximize(false)
			.AutoCenter(EAutoCenter::PrimaryWorkArea)
			// .MaxHeight(1000)
			// .MaxWidth(650)
			.MinHeight(800)
			.MinWidth(1200)
			.IsTopmostWindow(false)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(VersionUpdaterWidget,SVersionUpdaterWidget)
					.ToolName(FText::FromString(TOOL_NAME))
					.DeveloperName(FText::FromString(TEXT("bathurlu")))
					.DeveloperWebsite(FText::FromString(TEXT("https://bathur.cn")))
					.UpdateWebsite(FText::FromString(TEXT("https://bathur.cn")))
					.CurrentVersion(CURRENT_VERSION_ID)
				]
				+SVerticalBox::Slot()
				.FillHeight(1.0)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(0.3)
					[
						SNew(SBox)
						[
							SAssignNew(LauncherConfListPanel, SConfigListPanel)
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(0.7)
					[
						SAssignNew(LauncherPanel, SConfPanel)
					]
				]

				//.OnOpenedFileEvent.BindStatic(&WindowManager::OnOpenFileChangeWindowTitle)
			];
		LauncherConfListPanel->OnConfigFileSelected.BindStatic(&WindowManager::OnConfigSelected);
		LauncherPanel->OnAddToGlobal.BindStatic(&WindowManager::OnAddToGlobal);
		WindowManager::WindowsList.Add(ConfWindow);
		// show the window
		FSlateApplication::Get().AddWindow(ConfWindow.ToSharedRef());
		
		// Bind Opened file Event
		LauncherPanel->OnOpenedFileEvent.BindStatic(&WindowManager::OnOpenFileChangeWindowTitle);
		// Set Current Opened Conf File
		LauncherPanel->SetOpenedFile(ConfFile);
		// use config
		LauncherPanel->UpdateAll(Conf);

		return ConfWindow;
	}

	void OnOpenFileChangeWindowTitle(const FString& File)
	{
		if (!File.IsEmpty() && WindowManager::WindowsList.Num())
		{
			FString ConfFileName = EngineLaunchTools::GetFileNameByFullDir(File);
			WindowManager::WindowsList[0]->SetTitle(FText::FromString(ConfFileName + TEXT(" | UE Launcher")));
		}
		else
		{
			WindowManager::WindowsList[0]->SetTitle(FText::FromString(TEXT("UE Launcher")));
		}
	}
};

namespace CommandHandler
{
	void CreateConfWindowByLaunchParams(const FString& Param)
	{
		FLaunchConf Conf;
		FString jsonFile = Param;
		FString jsonValue(TEXT(""));
		FString JsonfullPath = FPaths::ConvertRelativePathToFull(jsonFile);
		if (FFileHelper::LoadFileToString(jsonValue, *JsonfullPath))
		{
			Conf = SerializationTools::DeSerializationConf(jsonValue);
		}
		// the config file is existed in disk?
		if (!FPaths::FileExists(jsonFile))
			jsonFile.Empty();
		CommandHandler::MainWindow = WindowManager::CreateConfWindow(Conf,jsonFile);
		if (CommandHandler::MainWindow.IsValid())
		{
			CommandHandler::HasWindow = true;
			CommandHandler::MainWindow->SetOnWindowClosed(FOnWindowClosed::CreateStatic(&CommandHandler::HandleMainWindowClosed));
		}
	}
	void LaunchConfInstanceByCmdLine(const FString& Param)
	{
		FLaunchConf Conf;
		FString jsonFile = Param;
		FString jsonValue(TEXT(""));
		FString JsonfullPath = FPaths::ConvertRelativePathToFull(jsonFile);
		if (FFileHelper::LoadFileToString(jsonValue, *JsonfullPath))
		{
			Conf = SerializationTools::DeSerializationConf(jsonValue);
		}
		EngineLaunchTools::EngineLauncher(Conf);
	}
	void GenerateLaunchConfFileByUproject(const FString& Param)
	{
		FLaunchConf Conf;
		FString EnginePath=EngineLaunchTools::GetUEProjectEnginePath(Param);
		Conf.Engine = EnginePath;
		Conf.Tool = TEXT("UE4Editor");
		Conf.Project = Param;

		FString ProjectDir = EngineLaunchTools::GetProjectDir(Param);
		FString ConfFileName = EngineLaunchTools::GetFileNameByFullDir(Param);

		for (uint32 Index = 0; Index < 100; ++Index)
		{
			FString IndexName = Index > 0 ? FString::Printf(TEXT("_%d"), Index) : TEXT("");
			FString SaveFile = FPaths::Combine(ProjectDir, TEXT("LaunchConf_")+ ConfFileName + IndexName + TEXT(".uejson"));
			if (!FPaths::FileExists(SaveFile))
			{	
				FFileHelper::SaveStringToFile(SerializationTools::SerializationConf(Conf), *SaveFile);
				break;
			}
		}
	}

	void HandleMainWindowClosed(const TSharedRef<SWindow>& pMainWindow)
	{
		// close the window notify user save config.
	}

	bool CmdKeyHasMatched(const TArray<FString>& pTokens, const TArray<FString>& pKeys)
	{
		bool bIsMatched = false;
		for (const auto& KeyIndex : pKeys)
		{
			if (pTokens.Find(KeyIndex)!=INDEX_NONE)
			{
				bIsMatched = true;
			}
		}

		return bIsMatched;
	}
};
#undef LOCTEXT_NAMESPACE