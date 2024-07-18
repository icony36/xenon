// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"

void UXeDamageAbility::CauseDamage(AActor* TargetActor) const
{
	// Create Effect Spec Handle.
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	// Get damage value from curved table.
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

	// Assign Tag and Damage for Set By Caller Magnitude.
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FXeGameplayTags::Get().Damage, ScaledDamage);

	// Apply damage to target.
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
	);
}

float UXeDamageAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());	
}
