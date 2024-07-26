// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAttributeSet.h"


void UXeAbility::CauseDamage(AActor* TargetActor, const bool bShouldUseDamageAttribute) const
{
	if (TargetActor == nullptr) return;

	// Create Effect Spec Handle.
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	float Damage = 0.f;

	if (bShouldUseDamageAttribute)
	{
		// Get damage from Damage Attribute.
		const FGameplayAttribute DamageAttribute = UXeAttributeSet::GetDamageAttribute();
		const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC->HasAttributeSetForAttribute(DamageAttribute))
		{
			Damage = ASC->GetNumericAttribute(DamageAttribute);
		}
	}
	else
	{
		// Get damage from curved table.
		Damage = SkillDamage.GetValueAtLevel(GetAbilityLevel());
	}

	// Assign Tag and Damage for Set By Caller Magnitude.
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FXeGameplayTags::Get().Data_Damage, Damage);

	// Apply damage Gameplay Effect to target.
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
	);
}

float UXeAbility::GetDamageAtLevel() const
{
	return SkillDamage.GetValueAtLevel(GetAbilityLevel());	
}
