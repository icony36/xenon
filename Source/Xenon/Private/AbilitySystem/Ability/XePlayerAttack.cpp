// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XePlayerAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"

void UXePlayerAttack::CauseDamage(AActor* TargetActor, const float Damage, const bool bShouldReactToHit,
                                  const FGameplayTag& HitReactTag) const
{
	if (TargetActor == nullptr) return;
	
	// Get ability system component of this ability.
	UXeAbilitySystemComponent* XeASC = Cast<UXeAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

	// Check if there is ability that handle the attack damage
	if (XeASC->HasAbilityWithTag(FXeGameplayTags::Get().Ability_HandleAttackDamage))
	{
		// Send gameplay event to trigger the other abilities.
		FGameplayEventData EventData;
		EventData.EventTag = FXeGameplayTags::Get().Event_Attack;
		EventData.Target = TargetActor;
		XeASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
	else
	{
		// Proceed to normal Cause Damage. 
		Super::CauseDamage(TargetActor, Damage, bShouldReactToHit, HitReactTag);
	}
}
