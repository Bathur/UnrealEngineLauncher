#pragma once

#include "Input/Reply.h"
#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FSelectorItemChanged,const FString&)

class SCombBoxWarper : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SCombBoxWarper)
		:_OnSelectorItemChanged()
	{}
	SLATE_EVENT(FSelectorItemChanged,OnSelectorItemChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:

	void HandleSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	TSharedRef<SWidget> HandleGenerateWidget(TSharedPtr<FString> InItem);
	FText HandleSelectionChangeText() const;

public:

	void UpdateSelector(const TArray<FString>& SelectorList,const FString& SelectedItem=TEXT(""));
	void ClearAllSelectItem();
	FString GetCurretSelectedItem()const;

private:

	FSelectorItemChanged OnSelectorItemChanged;
	TSharedPtr<FString>	CurrentSelectedItem;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> SelectorWidget;
	TArray<TSharedPtr<FString> > SelectorTextList;

};
