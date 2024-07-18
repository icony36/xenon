// Copyright CoNY. All rights reserved.


#include "Input/XeInputTagConfig.h"

const UInputAction* UXeInputTagConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FXeInputProperties& Props: AbilityInputProps)
	{
		if (Props.InputAction && Props.InputTag.MatchesTag(InputTag))
		{
			return Props.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find InputAction for InputTag [%s], on InputTagConfing [%s"), *InputTag.ToString(), * GetNameSafe(this));
	}

	return nullptr;
}
