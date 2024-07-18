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
	UFUNCTION(BlueprintCallable, Category="DamageAbility")
	virtual void CauseDamage(AActor* TargetActor) const;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;
};
