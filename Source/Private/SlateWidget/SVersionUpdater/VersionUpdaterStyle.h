// Copyright 2019 Lipeng Zha, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FVersionUpdaterStyle
{

public:

	static void Initialize();

	static void Shutdown();

	/** Reloads Textures Used By Slate Renderer */
	static void ReloadTextures();

	/** Return The Slate Style Set */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

	static const FSlateBrush* GetBrush( FName PropertyName, const ANSICHAR* Specifier = NULL );

private:

	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;

};
