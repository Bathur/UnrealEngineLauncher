#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Data/FLaunchConf.h"
#include "Tools/EngineLaunchTools.h"

class SEditableBoxWrapper;
class SConfPanel;

DECLARE_DELEGATE_OneParam(FOnOpendFile, const FString&)
DECLARE_DELEGATE_OneParam(FOnAddToGlobal, FLaunchConf)

class SConfPanel :public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SConfPanel)
		:_OnOpenedFileEvent()
	{ }

	SLATE_EVENT(FOnOpendFile, OnOpenedFileEvent)
	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

	// Slate Event
	FOnOpendFile OnOpenedFileEvent;

public:

	// Widget Show Text
	FText GetProjectFileText()const;
	FText GetLaunchEngineBtnText()const;
	FText GetLaunchProjectBtnText()const;

public:

	// Get Conf
	FString GetSelectedEnginePath()const;
	// Get Selected .uproject File Path
	FString GetSelectedProjectPath()const;
	// Get Selected Tool
	FString GetSelectedTool()const;
	// Get All Launch Parameters
	TArray<FString> GetAllLaunchParams()const;
	// Get All Conf
	FLaunchConf GetLaunchConf()const;

public:

	// Add/Clear Launch Parameter Button Clicked Event
	FReply BtnClickEventAddLaunchParamButton();
	FReply BtnClickEventClearAllLaunchParamsButton();
	// Launch Project
	FReply BtnClickEventLaunchProject();
	// Lcunch Engine
	FReply BtnClickEventLaunchEngine();
	FReply BtnClickEventOpenVS();
	FReply BtnClickEventOpenEngineDir();
	// Open .uproject File Clicked Event
	FReply BtnClickEventOpenProjectFile();
	// Open Project Directory
	FReply BtnClickEventOpenProjectFileDir();
	// Open Project .sln File
	FReply BtnClickEventOpenProjectSln();
	FReply BtnClickEventGenerateProjectSln();
	// Read/Write Config
	FReply BtnClickEventLoadConfig();
	FReply BtnClickEventSaveConfig();
	FReply BtnClickEventAddToGlobal();
	FReply BtnClickEventClearConfig();
	// Open Developer Website
	void HyLinkClickEventOpenDeveloperWebsite();

	EVisibility BtnGenerateSlnVisibility()const;
	void SetOpenedFile(const FString& Path=TEXT(""));
	FString GetOpenedFile()const;

public:

	// State Changed Event
	void OnProjectFileTextBoxChanged(const FText& NewText);

public:

	// Update Engine Selector
	void UpdateEngineSelector(const TMap<FString, FString>& pEngineMap, FString DefaultEngine = TEXT(""));
	// Update Tools Selector
	void UpdateToolSelector(const TArray<FString>& ToolsList,const FString& DefaultTool=TEXT(""));
	// Update Is Show OpenVS Button
	void UpdateOpenVSButton(const FString& EnginePath);
	
	void UpdateSelectedProject(const FString& ProjectPath=TEXT(""));
	// Update Is Show OpenVS Button
	void UpdateOpenProjectSlnButton(const FString& SelectedProjectPath);

	void UpdateLaunchParams(const TArray<FString>& pParamsArray = TArray<FString>{});

	// Update All
	void UpdateAll(const FLaunchConf& conf);

protected:

	// Create/Add A Editable Parameter Box.
	TSharedRef<SEditableBoxWrapper> CreateEditableTextBox(const FString& TextContent);
	void AddParamTextBoxToSlot(const FString& TextContent=TEXT(""));

public:

	void HandleEngineSelectorChanged(const FString& NewEngine);

	FOnAddToGlobal OnAddToGlobal;

private:

	// Main Panel Scrollbox
	TSharedPtr<SScrollBox> SrbWidgetMain;
	// Selector
	TSharedPtr<class SCombBoxWarper> CmbEngineSelector;
	TSharedPtr<class SCombBoxWarper> CmbToolSelector;
	// Add Launch Parameter
	TSharedPtr<SScrollBox> SrbWidgetLaunchParams;
	// Buttons
	TSharedPtr<SButton> BtnLaunchEngine;
	TSharedPtr<SButton> BtnOpenVS;
	TSharedPtr<SButton> BtnGenerateSln;
	TSharedPtr<SButton> BtnOpenProjectSln;
	TSharedPtr<SButton> BtnLaunchProject;

	// Buttons Text
	mutable FString LaunchEngineBtnText{ TEXT("Launch") };
	mutable FString LaunchProjectBtnText{ TEXT("Launch Configuration") };

	// opened .uproject file
	FString OpenProjectFilePath;

private:

	// General Data
	TMap<FString, FString> RegisterEngineMap;

	FString OpenedConfFile;

};
