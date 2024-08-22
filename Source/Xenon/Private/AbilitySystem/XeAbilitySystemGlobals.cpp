// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemGlobals.h"

#include "AbilitySystem/XeGameplayEffectTypes.h"

FGameplayEffectContext* UXeAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	// Use our custom Gameplay Effect Context.
	return new FXeGameplayEffectContext();
}
