#include "Tools/SlateWidgetTools.h"
#include "Tools/HackPrivateMember.hpp"

// Engine Files
#include "Widgets/Layout/SScrollBox.h"

DECL_HACK_PRIVATE_DATA(SScrollBox, TSharedPtr<SScrollPanel>, ScrollPanel)

FChildren* SlateWidgetTools::GetScrollBoxChildren(SScrollBox* ScrollboxWidget)
{
	SPanel* ScrollBoxMemScrollPanel = (SPanel*)(&*(GET_VAR_PRIVATE_DATA_MEMBER(ScrollboxWidget, SScrollBox, ScrollPanel)));
	FChildren* ScrollBoxChildren = ScrollBoxMemScrollPanel->GetChildren();
	return ScrollBoxChildren;
}
