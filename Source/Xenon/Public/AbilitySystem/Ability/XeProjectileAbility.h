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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AXeProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category="XeAbility|Projectile")
	void SpawnProjectile(FRotator& SpawnRotation, const FGameplayTag& SpawnSocketTag, bool bShouldOverridePitch, float PitchOverride = 0.f);
};
