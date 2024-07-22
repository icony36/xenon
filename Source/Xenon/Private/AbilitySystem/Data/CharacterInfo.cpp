// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Data/CharacterInfo.h"

FCharacterDefaultInfo UCharacterInfo::GetCharacterDefaultInfo(const FGameplayTag& CharacterTag)
{
	return CharacterClasses.FindChecked(CharacterTag);
}
