// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemGlobals.h"

#include "AbilitySystem/XeGameplayEffectTypes.h"

FGameplayEffectContext* UXeAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FXeGameplayEffectContext();
}
