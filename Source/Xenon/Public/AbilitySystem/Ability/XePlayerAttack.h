// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/XeDamageAbility.h"
#include "XePlayerAttack.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXePlayerAttack : public UXeDamageAbility
{
	GENERATED_BODY()

protected:
	virtual void CauseDamage(AActor* TargetActor, const float Damage, const bool bShouldReactToHit, const FGameplayTag& HitReactTag) const override;
};
