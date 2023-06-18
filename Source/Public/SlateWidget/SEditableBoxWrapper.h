#pragma once

#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SEditableBoxWrapper;

TSharedPtr<SEditableBoxWrapper> MakeShareableEditBox(SEditableBoxWrapper* EditableBoxWrapperObj);

DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnDeleteClicked, TSharedPtr<SEditableBoxWrapper>)

class SEditableBoxWrapper : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SEditableBoxWrapper)
		:_EditableText()
		,_EditableHintText()
		,_BtnOpenText()
		,_BtnClearText()
		,_BtnDeleteText()
	{}
	SLATE_ATTRIBUTE(FText, EditableText)
	SLATE_ATTRIBUTE(FText, EditableHintText)
	SLATE_ATTRIBUTE(FText, BtnOpenText)
	SLATE_ATTRIBUTE(FText, BtnClearText)
	SLATE_ATTRIBUTE(FText, BtnDeleteText)
	SLATE_EVENT(FOnDeleteClicked, OnDeleteClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetText(const FText& NewText);
	FText GetEditableBoxText()const;
	FOnDeleteClicked OnDeleteSelgClicked;

	FReply OnClickEventClearText();
	FReply OnClickEventOpenText();
	FReply OnClickEventDeleteSelf();

private:

	TSharedPtr<SHorizontalBox> HorzontaBox;
	TSharedPtr<SEditableTextBox> EditableTextBox;

};
