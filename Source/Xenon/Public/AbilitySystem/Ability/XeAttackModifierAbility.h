// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/XeAbility.h"
#include "XeAttackModifierAbility.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeAttackModifierAbility : public UXeAbility
{
	GENERATED_BODY()

public:
	
	/**
	 * Seperate 1 Damage out parameter into 2 to address issues with Blueprint input and output pins.
	 * @param InDamage Damage to modify, should be the same as OutDamage.
	 * @param OutDamage Damage to modify, should be the same as InDamage.
	 * @return True if successfully modify attack.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Attack Modifier")
	bool ApplyAttackModifier(UPARAM(ref) float& InDamage, float& OutDamage);
};
