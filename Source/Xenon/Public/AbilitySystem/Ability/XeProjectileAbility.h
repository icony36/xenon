// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/XeAbility.h"
#include "XeProjectileAbility.generated.h"

class AXeProjectile;
/**
 * 
 */
UCLASS()
class XENON_API UXeProjectileAbility : public UXeAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<AXeProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float Range = 100.f;

	UFUNCTION(BlueprintCallable, Category="XeAbility|Projectile")
	void SpawnProjectile(const FGameplayTag& SpawnSocketTag, bool bShouldOverridePitch, float PitchOverride = 0.f);
};
