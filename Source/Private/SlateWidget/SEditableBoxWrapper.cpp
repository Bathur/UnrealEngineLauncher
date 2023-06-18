#include "SlateWidget/SEditableBoxWrapper.h"
#include "SlateOptMacros.h"
#include "Tools/EngineLaunchTools.h"

#define LOCTEXT_NAMESPACE "EditableBoxWrapper"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEditableBoxWrapper::Construct(const FArguments& InArgs)
{
	OnDeleteSelgClicked = InArgs._OnDeleteClicked;
	ChildSlot
		[
			SAssignNew(HorzontaBox, SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SAssignNew(EditableTextBox,SEditableTextBox)
				.Text(InArgs._EditableText)
				.HintText(InArgs._EditableHintText)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(InArgs._BtnOpenText)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SEditableBoxWrapper::OnClickEventOpenText)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(InArgs._BtnClearText)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SEditableBoxWrapper::OnClickEventClearText)
			]
			// Open .uproject File Button
			+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(InArgs._BtnDeleteText)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this,&SEditableBoxWrapper::OnClickEventDeleteSelf)
				]
	];
	
}

FReply SEditableBoxWrapper::OnClickEventClearText()
{
	EditableTextBox->SetText(FText::FromString(TEXT("")));
	return FReply::Handled();
}

FReply SEditableBoxWrapper::OnClickEventOpenText()
{
	TArray<FString> OpenFiles = EngineLaunchTools::OpenFileDialog();
	FString AppendText;
	for(const auto& File:OpenFiles)
	{
		AppendText += FPaths::ConvertRelativePathToFull(File);
	}
	
	EditableTextBox->SetText(FText::FromString(EditableTextBox->GetText().ToString() + AppendText));
	return FReply::Handled();
}

FReply SEditableBoxWrapper::OnClickEventDeleteSelf()
{
	EditableTextBox->SetText(FText::FromString(TEXT("")));
	this->SetVisibility(EVisibility::Collapsed);
	return FReply::Handled();
}

void SEditableBoxWrapper::SetText(const FText& NewText)
{
	EditableTextBox->SetText(NewText);
}

FText SEditableBoxWrapper::GetEditableBoxText()const
{
	return EditableTextBox->GetText();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
