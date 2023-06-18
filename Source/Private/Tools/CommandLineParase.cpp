#include "Tools/CommandLineParase.h"
#include <cctype>

TMap<FString,FString> CommandLineParase::GetCommandLineParamsMap(const FString& CommandLine)
{
	TMap<FString,FString> result;
	TArray<FString> ParamsSwitchs,ParamsTokens;
	FCommandLine::Parse(*CommandLine, ParamsTokens, ParamsSwitchs);

	for(const auto& SwitchItem:ParamsSwitchs)
	{
		if (!SwitchItem.IsEmpty() && ParamsTokens.Num() && ParamsTokens.Find(SwitchItem) != INDEX_NONE)
		{
			FString ItemToken(TEXT(""));
			for (int32 index = ParamsTokens.Find(SwitchItem)+1;
				index < ParamsTokens.Num() && ParamsSwitchs.Find(ParamsTokens[index]) == INDEX_NONE;
				++index)
			{
				ItemToken.Append(ParamsTokens[index]).Append(TEXT(" "));
			}
			if (!ItemToken.IsEmpty())
			{
				// Remove All End Point Space
				while (std::isspace(ItemToken[ItemToken.Len() - 1]))
					ItemToken.RemoveAt(ItemToken.Len() - 1);
			}
			result.Add(SwitchItem,ItemToken);
		}
	}

	return result;
}
