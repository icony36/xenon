// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "XeAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// To use our custom Gameplay Effect Context.
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
