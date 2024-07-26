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

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<AXeProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float Range = 100.f;

	UFUNCTION(BlueprintCallable, Category="XeAbility|Projectile")
	void SpawnProjectile(FRotator SpawnRotation, const FGameplayTag& SpawnSocketTag, bool bShouldOverridePitch, float PitchOverride = 0.f);
};
