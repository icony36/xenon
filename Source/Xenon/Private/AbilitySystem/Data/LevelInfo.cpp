// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Data/LevelInfo.h"

int32 ULevelInfo::FindLevelWithEXP(int32 EXP) const
{
	int32 CurrentLevel = 1;
	
	bool bSearching = true;
	while(bSearching)
	{
		// Return early if current is greater than stored level. Minus 1 as LevelUpInformation[0] is no level (placeholder).
		if (LevelUpInformation.Num() - 1 <= CurrentLevel) return CurrentLevel;

		if (EXP >= LevelUpInformation[CurrentLevel].LevelUpRequirement)
		{
			CurrentLevel++;
		}
		else
		{
			bSearching = false;
		}
	}

	return CurrentLevel;
}
