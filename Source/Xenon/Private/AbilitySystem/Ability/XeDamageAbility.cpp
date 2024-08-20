// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemLibrary.h"
#include "AbilitySystem/XeAttributeSet.h"

float UXeDamageAbility::GetDamageAtLevel(const bool bShouldUseDamageAttribute) const
{
	// Get damage from Damage Attribute.
	if (bShouldUseDamageAttribute)
	{
		const FGameplayAttribute DamageAttribute = UXeAttributeSet::GetDamageAttribute();
		const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC->HasAttributeSetForAttribute(DamageAttribute))
		{
			return ASC->GetNumericAttribute(DamageAttribute);
		}
	}

	// Get damage from curved table.
	return SkillDamage.GetValueAtLevel(GetAbilityLevel());
}

void UXeDamageAbility::CauseDamage(AActor* TargetActor, const float Damage, const bool bShouldReactToHit,
	const FGameplayTag& HitReactTag) const
{
	if (TargetActor == nullptr) return;

	// Create Effect Spec Handle.
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	// Set Hit React and Hit Direction if target should react to hit.
	if (bShouldReactToHit)
	{
		FGameplayEffectContextHandle EffectContextHandle = DamageSpecHandle.Data->GetEffectContext();
		UXeAbilitySystemLibrary::SetHitReactTag(EffectContextHandle, HitReactTag);
	}

	// Assign Tag and Damage for Set By Caller Magnitude.
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		DamageSpecHandle,
		FXeGameplayTags::Get().Data_Damage,
		Damage);

	// Apply damage Gameplay Effect to target.
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
