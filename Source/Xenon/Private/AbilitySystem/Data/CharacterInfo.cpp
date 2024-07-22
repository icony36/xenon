// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Data/CharacterInfo.h"

FCharacterProperties UCharacterInfo::GetCharacterProperties(const FGameplayTag& CharacterTag)
{
	return CharacterClasses.FindChecked(CharacterTag);
}
