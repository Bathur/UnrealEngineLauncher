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

// Project Files
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
	// Launch Config Don't Create Config Panel
	void LaunchConfInstanceByCmdLine(const FString& Param);
	// From .uproject Create .uejson
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
	// Add .uejson File Mapping For Administrator User
	EngineLaunchTools::UnrealEngineLauncherRegisterWriter();

	FCommandLine::Set(pCmdLine);
	// Start Up The Main Loop
	GEngineLoop.PreInit(pCmdLine);
	// Make Dure Sll UObject Classes Are Registered And Default Properties Have Been Initialized
	ProcessNewlyLoadedUObjects();
	// Tell The Module Manager It May Now Process Newly-loaded UObjects When New C++ Modules Are Loaded
	FModuleManager::Get().StartProcessingNewlyLoadedObjects();
	// Crank Up A Normal Slate Application Using The Platform's Standalone Renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());
	// Set The Application Name
	FGlobalTabmanager::Get()->SetApplicationTitle(LOCTEXT("AppTitle", "Unreal Engine Launcher"));
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
		// Main Loop
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
			.Title(LOCTEXT("MainWindow_Title", "Unreal Engine Launcher"))
			.ScreenPosition(FVector2D(800, 1200))
			.ClientSize(FVector2D(1200, 800))
			.SupportsMaximize(false)
			.AutoCenter(EAutoCenter::PrimaryWorkArea)
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
					.DeveloperName(FText::FromString(TEXT("BathurLu")))
					.DeveloperWebsite(FText::FromString(TEXT("https://imzlp.com")))
					.UpdateWebsite(FText::FromString(TEXT("https://imzlp.com/posts/11750/")))
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
			];

		LauncherConfListPanel->OnConfigFileSelected.BindStatic(&WindowManager::OnConfigSelected);
		LauncherPanel->OnAddToGlobal.BindStatic(&WindowManager::OnAddToGlobal);
		WindowManager::WindowsList.Add(ConfWindow);
		// Show The Window
		FSlateApplication::Get().AddWindow(ConfWindow.ToSharedRef());
		
		// Bind Opened File Event
		LauncherPanel->OnOpenedFileEvent.BindStatic(&WindowManager::OnOpenFileChangeWindowTitle);
		// Set Current Opened Conf File
		LauncherPanel->SetOpenedFile(ConfFile);
		// Use Config
		LauncherPanel->UpdateAll(Conf);

		return ConfWindow;
	}

	void OnOpenFileChangeWindowTitle(const FString& File)
	{
		if (!File.IsEmpty() && WindowManager::WindowsList.Num())
		{
			FString ConfFileName = EngineLaunchTools::GetFileNameByFullDir(File);
			WindowManager::WindowsList[0]->SetTitle(FText::FromString(ConfFileName + TEXT(" | Unreal Engine Launcher")));
		}
		else
		{
			WindowManager::WindowsList[0]->SetTitle(FText::FromString(TEXT("Unreal Engine Launcher")));
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
		// The Config File Is Existed In Disk?
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
		// Close The Window Notify User Save Config.
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
