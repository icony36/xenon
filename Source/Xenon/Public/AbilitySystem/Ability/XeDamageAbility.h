// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/XeAbility.h"
#include "XeDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeDamageAbility : public UXeAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="XeAbility")
	float GetDamageAtLevel(const bool bShouldUseDamageAttribute = false) const;
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="XeAbility")
	virtual void CauseDamage(AActor* TargetActor, const float Damage, const bool bShouldReactToHit = false, const FGameplayTag& HitReactTag = FGameplayTag()) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat SkillDamage;
};
